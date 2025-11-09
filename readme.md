# AI Chat Pet Robot

An intelligent voice-controlled robot using ESP32-S3 with real-time AI conversation capabilities and autonomous movement.

## 🤖 Project Overview

AI Chat Pet is a smart robot that combines voice recognition, AI chat, and motor control. It can understand spoken commands, engage in conversations, and move around autonomously using local LLM processing.

## 🛠 Hardware Components

| Component | Model | Purpose |
|-----------|-------|---------|
| Main Board | ESP32-S3 DevKit | Core controller |
| Microphone | INMP441 | Voice input |
| Audio Amplifier | MAX9835A | Voice output |
| Motor Driver | L298N | Motor control |
| Display | OLED SSD1306 | Status display |
| Motors | DC Motors x2 | Wheel drive |
| RGB LED | NeoPixel | Visual feedback |

## 📁 Project Structure

### Core Hardware Control
- **`i2s_manager.h`** - Audio I/O management (INMP441 mic & MAX98357A DAC)
- **`robot_movement.h`** - L298N motor control for 2-wheel movement
- **`oled_display.h`** - SSD1306 display with emotional face animations
- **`RGB.h`** - NeoPixel LED status indicators

### AI & Communication
- **`llm_client.h`** - ESP32 client for Ollama LLM communication
- **`main.py`** - Main AI server with STT/TTS and robot control
- **`AI_Personality.py`** - Character personas (Sophia/小雅) and conversation rules

### Utilities
- **`cmd.py`** - Serial control for manual robot testing
- **`llm.py`** - Python LLM client for testing

## 🔧 Key Features

### Voice Control
- **Speech Recognition**: Vosk-based STT with Chinese/English support
- **Voice Synthesis**: Edge-TTS with multiple voice options
- **Natural Commands**: "前进", "back", "左转", "right", "stop"

### AI Conversation
- **Local LLM**: Ollama integration (Gemma 2B, Phi-3, etc.)
- **Dual Personalities**: Sophia (English) and 小雅 (Chinese)
- **Context Awareness**: Maintains conversation context

### Robot Movement
- **Motor Control**: Forward, backward, left/right turns
- **Autonomous Navigation**: Voice-controlled movement
- **Status Feedback**: OLED faces and RGB LED indicators

## 🚀 Quick Start

### Hardware Setup
```
INMP441 Microphone:
  DATA -> GPIO16, BCLK -> GPIO18, LRCK -> GPIO17

MAX98357A DAC:
  DATA -> GPIO7, BCLK -> GPIO5, LRCK -> GPIO6

L298N Motor Driver:
  IN1->GPIO4, IN2->GPIO10, IN3->GPIO11, IN4->GPIO12

OLED Display: I2C 0x3C
RGB LED: GPIO48
```

### Software Installation
```bash
# Install Python dependencies
pip install vosk requests edge-tts pydub

# Install Ollama (https://ollama.ai/)
curl -fsSL https://ollama.ai/install.sh | sh
ollama pull gemma:2b

# Run AI server
python main.py
```

### ESP32 Setup
1. Upload Arduino code to ESP32-S3
2. Configure WiFi in `llm_client.h`
3. Connect to AI server via sockets

## 📡 Communication Protocol

### STT Channel (Port 5005)
- Receives raw audio from ESP32
- Returns recognized text

### TTS Channel (Port 5006)
- Sends control commands: `!CTRL#COMMAND#END`
- Sends audio pause: `!PAUSE_STT#DURATION#END`
- Streams PCM16 audio data

## 🎮 Usage

### Voice Commands
- **Movement**: "前进", "back", "left", "right", "stop"
- **Conversation**: Natural chat in Chinese/English
- **Control**: Combines movement with verbal feedback

### Manual Control
```bash
python cmd.py
# Commands: f(forward), b(backward), l(left), r(right), q(quit)
```

## 🔧 Configuration

### AI Personalities
Edit `AI_Personality.py` to customize:
- Character names and ages
- Conversation style and rules
- Language preferences

### Voice Settings
Modify `main.py` for:
- TTS voice selection (XiaoxiaoNeural, AriaNeural, etc.)
- STT model path
- Audio sample rates

## 🛠 Troubleshooting

### Common Issues
1. **Audio not working**: Check I2S pin connections
2. **Motors not moving**: Verify L298N power and enable pins
3. **WiFi connection failed**: Update credentials in `llm_client.h`
4. **LLM not responding**: Ensure Ollama server is running

### Debug Modes
- Serial monitor for ESP32 debugging
- Audio file saving for TTS verification
- OLED status display for connection state

## 📄 License

MIT License - see LICENSE file for details.

## 🙏 Acknowledgments

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [Vosk Speech Recognition](https://alphacephei.com/vosk/)
- [Ollama](https://ollama.ai/) for local LLM
- [Edge-TTS](https://github.com/rany2/edge-tts) for voice synthesis

---

**Project by**: [Your Name]  
**Contact**: [Your Email]  
**Repository**: [Your GitHub Link]
