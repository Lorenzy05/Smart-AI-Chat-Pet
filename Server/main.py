import socket
import threading
import vosk
import json
import requests
import asyncio
import edge_tts
import time
import os
import wave
from pydub import AudioSegment

# === 配置 ===
# D:\MS\vosk-model-cn-0.22\vosk-model-cn-0.22
# D:\MS\vosk-model-en-us-0.22
# C:\vosk-model-small-cn-0.22
MODEL_STT_PATH = r"D:\MS\vosk-model-cn-0.22\vosk-model-cn-0.22"
OLLAMA_HOST = "http://127.0.0.1:11434"
#OLLAMA_MODEL = "gemma:2b"  
#OLLAMA_MODEL = "phi3:3.8b-instruct"
#OLLAMA_MODEL = "starling-lm:7b-beta"
OLLAMA_MODEL = "gemma:2b"
STT_PORT = 5005
TTS_PORT = 5006
# === 保存路径 ===
SAVE_WAV_PATH = os.path.join(os.path.expanduser("~/Desktop"), "tts_output")
os.makedirs(SAVE_WAV_PATH, exist_ok=True)  # 自动创建文件夹telnet 192.168.178.91 5005


# === STT 模型 ===
model = vosk.Model(MODEL_STT_PATH)
recognizer = vosk.KaldiRecognizer(model, 16000)
tts_conn = None  # 全局 TTS 连接

def is_control_command(text):
    """检查是否为控制指令"""
    control_keywords = ["前进", "后退", "左边", "右边", "左转", "右转", "停止", 
                       "forward", "back", "left", "right", "stop", "过来"]
    return any(keyword in text for keyword in control_keywords)

def get_control_command(text):
    """提取控制指令"""
    if "前进" in text or "过来" in text or "forward" in text.lower():
        return "FORWARD", "好的，前进"
    elif "后退" in text or "back" in text.lower():
        return "BACKWARD", "好的，后退"
    elif "左边" in text or "左转" in text.lower() or "left" in text.lower():
        return "LEFT", "好的，左转"
    elif "右边" in text or "右转" in text.lower() or "right" in text.lower():
        return "RIGHT", "好的，右转"
    return None, None

# === Edge-TTS 异步生成 PCM16 LE 数据 ===
# === Edge-TTS 异步生成 MP3 ===
#"en-US-AriaNeural" （最常用的美国女声，温柔自然） 
#"en-US-JennyNeural" （年轻女生风格，活泼一点）
#"en-GB-LibbyNeural" （英式女声，清晰）
# zh-CN-XiaoxiaoNeural → 最常用，年轻女声，温柔自然。
# zh-CN-XiaoyiNeural → 稍微成熟稳重，客服/主持风格。
# zh-CN-XiaohanNeural → 女声，带点青春活泼感。
# zh-CN-XiaomengNeural → 女声，偏甜美，适合小孩互动。
# zh-CN-XiaoruiNeural → 女声，偏冷静、专业

async def edge_tts_generate(text: str) -> bytes:
    communicate = edge_tts.Communicate(
        text=text, 
        voice="zh-CN-XiaoxiaoNeural"
    )
    mp3_bytes = b""
    async for chunk in communicate.stream():
        if chunk['type'] == 'audio':
            mp3_bytes += chunk['data']
    return mp3_bytes

def generate_tts_sync(text: str) -> bytes:
    mp3_bytes = asyncio.run(edge_tts_generate(text))

    # 保存 MP3
    mp3_path = os.path.join(SAVE_WAV_PATH, f"tts_{int(time.time())}.mp3")
    with open(mp3_path, "wb") as f:
        f.write(mp3_bytes)

    # 转成 WAV PCM16
    audio = AudioSegment.from_file(mp3_path, format="mp3")
    wav_path = mp3_path.replace(".mp3", ".wav")
    audio.export(wav_path, format="wav", parameters=["-acodec", "pcm_s16le", "-ar", "16000", "-ac", "1"])
    print(f"[TTS] 已保存 WAV 文件: {wav_path}")

    # 直接返回 PCM16 数据
    return audio.set_frame_rate(16000).set_channels(1).set_sample_width(2).raw_data

# === LLM + TTS 处理线程 ===
from AI_Personality import basic_data_cn
from AI_Personality import basic_data_en


basic_data = basic_data_cn

def llm_tts_thread(prompt: str):
    global tts_conn
    try:
        # === 首先检查是否为控制指令 ===
        if is_control_command(prompt):
            control_cmd, tts_response = get_control_command(prompt)
            
            if control_cmd:
                print(f"[控制指令] {control_cmd}")
                # 发送控制命令给 ESP32
                control_msg = f"!CTRL#{control_cmd}#END\n"
                if tts_conn:
                    tts_conn.sendall(control_msg.encode())
                    time.sleep(0.1)
                
                # 生成简单的语音反馈
                if tts_response:
                    # 在发送TTS前，告诉ESP32暂停STT发送（5秒）
                    pause_msg = "!PAUSE_STT#5000#END\n"
                    if tts_conn:
                        tts_conn.sendall(pause_msg.encode())
                        time.sleep(0.1)
                    
                    pcm16_bytes = generate_tts_sync(tts_response)
                    print(f"[TTS] 生成 {len(pcm16_bytes)} 字节音频数据")
                    
                    # 发送 TTS 音频
                    if tts_conn:
                        chunk_size = 1024
                        for i in range(0, len(pcm16_bytes), chunk_size):
                            tts_conn.sendall(pcm16_bytes[i:i+chunk_size])
                            time.sleep(0.01)
                    
                    print(f"[TTS] 控制指令音频发送完成")
                
                # ===== 关键修复：在这里立即返回！ =====
                return  # 控制指令处理完成，不再执行后面的LLM代码
                # ====================================
        
        # === 如果不是控制指令，才发送给 LLM ===
        print(f"[LLM] 处理非指令对话: {prompt}")
        full_prompt = str(basic_data) + str(prompt)
        
        url = f"{OLLAMA_HOST}/api/generate"
        data = {"model": OLLAMA_MODEL, "prompt": full_prompt, "stream": False}
        response = requests.post(url, json=data).json()
        answer = response.get("response", "")
        answer = answer[3:] if answer.startswith("小雅：") else answer
        if not answer.strip():
            print("[LLM] 收到空回复")
            return
        print("[LLM]", answer)
        
    except Exception as e:
        print("LLM 请求失败:", e)
        return

    # === TTS 处理 ===
    try:
        # 在发送TTS前，告诉ESP32暂停STT发送（10秒）
        pause_msg = "!PAUSE_STT#10000#END\n"
        if tts_conn:
            tts_conn.sendall(pause_msg.encode())
            time.sleep(0.1)
        
        pcm16_bytes = generate_tts_sync(answer)
        print(f"[TTS] 生成 {len(pcm16_bytes)} 字节音频数据")
        
    except Exception as e:
        print("TTS 生成失败:", e)
        return

    # === 发送给 ESP32 ===
    try:
        while tts_conn is None:
            print("等待 TTS 连接...")
            time.sleep(0.1)
        
        chunk_size = 1024
        total_sent = 0
        for i in range(0, len(pcm16_bytes), chunk_size):
            chunk = pcm16_bytes[i:i+chunk_size]
            tts_conn.sendall(chunk)
            total_sent += len(chunk)
            time.sleep(0.01)
        
        print(f"[TTS] 成功发送 {total_sent} 字节音频数据")
        
    except Exception as e:
        print("TTS 发送失败:", e)



# === STT 接收线程 ===
def stt_thread(conn):
    try:
        while True:
            data = conn.recv(1024)
            if not data:
                break
            if recognizer.AcceptWaveform(data):
                text = json.loads(recognizer.Result())["text"]
                if text.strip():
                    print("[STT]", text)
                    threading.Thread(target=llm_tts_thread, args=(text,)).start()
    finally:
        conn.close()

# === TTS 服务器线程 ===
def tts_server():
    global tts_conn
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('0.0.0.0', TTS_PORT))
    sock.listen(1)
    print("等待 ESP32 连接 (TTS)...")
    tts_conn, addr = sock.accept()
    print("TTS 已连接:", addr)
    try:
        while True:
            data = tts_conn.recv(1)
            if not data:
                break
    except Exception:
        pass
    finally:
        tts_conn.close()
        tts_conn = None

def main():
    threading.Thread(target=tts_server, daemon=True).start()
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.bind(('0.0.0.0', STT_PORT))
    sock.listen(1)
    print("等待 ESP32 连接 (STT)...")
    conn, addr = sock.accept()
    print("STT 已连接:", addr)
    stt_thread(conn)

if __name__ == "__main__":
    main()
