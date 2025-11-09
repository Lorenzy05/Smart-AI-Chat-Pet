import requests
import json

OLLAMA_HOST = "http://192.168.178.91:11434"
MODEL = "gemma:2b"  # 替换成 Ollama 模型名字

def send_prompt_stream(prompt: str):
    url = f"{OLLAMA_HOST}/api/generate"
    headers = {"Content-Type": "application/json"}
    data = {
        "model": MODEL,
        "prompt": prompt,
        "stream": True
    }

    full = ""

    with requests.post(url, headers=headers, json=data, stream=True) as r:
        r.raise_for_status()
        for line in r.iter_lines(decode_unicode=True):
            if line:
                try:
                    obj = json.loads(line)
                    if "response" in obj:
                        token = obj["response"]
                        print(token, end="", flush=True)  # 实时打印
                        full = str(full) + str(token)
                except json.JSONDecodeError:
                    pass  # 忽略非 JSON 行
