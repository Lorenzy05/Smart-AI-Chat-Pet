## 📁 i2s_manager.h

```markdown
# I2S Manager - Audio Input/Output Handler

## Overview
Manages I2S audio communication for INMP441 microphone (input) and MAX98357A DAC (output). Handles PCM data streaming between microphone and speaker.

## Key Functions

### Initialization
- `begin()` - Initializes both microphone and DAC
- `initMicrophone()` - Configures INMP441 microphone with I2S
- `initDAC()` - Configures MAX98357A audio amplifier

### Audio Processing
- `readMicrophone()` - Reads PCM data from microphone
- `writeDAC()` - Writes PCM data to speaker
- `processMicrophoneSample()` - Converts 32-bit to 16-bit audio with gain
- `convertMicBuffer()` - Batch processes microphone buffer

### Utilities
- `printAudioStats()` - Debug audio levels and statistics
- `loopbackOnce()` - Real-time audio passthrough test

## Audio Specifications
- **Sample Rate**: 16kHz
- **Microphone**: 32-bit samples (INMP441)
- **DAC**: 16-bit samples (MAX98357A)
- **Buffer Size**: 256 samples

## Pin Configuration
```
INMP441 Microphone:
  DATA -> GPIO16
  BCLK -> GPIO18  
  LRCK -> GPIO17

MAX98357A DAC:
  DATA -> GPIO7
  BCLK -> GPIO5
  LRCK -> GPIO6
```

## Usage Example
```cpp
I2SManager audio;
audio.begin();

// Read from microphone
int32_t micBuffer[256];
size_t samples = audio.readMicrophone(micBuffer, 256);

// Process and play audio
int16_t dacBuffer[256];
audio.convertMicBuffer(micBuffer, dacBuffer, samples);
audio.writeDAC(dacBuffer, samples);
```
```

## 📁 llm_client.h

```markdown
# LLM Client - AI Chat Interface

## Overview
Provides communication with Ollama LLM server for AI chat functionality. Supports both streaming and non-streaming responses.

## Key Components

### WiFi Configuration
- Pre-configured WiFi credentials
- Ollama server connection settings

### Core Functions
- `sendPromptStreamToOllama()` - Stream response from Ollama with real-time output
- `OllamaClient` class - Object-oriented interface for LLM communication

## Features
- **Streaming Responses**: Real-time token-by-token output
- **JSON Parsing**: Automatic extraction of response text
- **Error Handling**: Connection failure detection
- **Text Processing**: Handles escape sequences and formatting

## Communication Protocol
- **HTTP POST** to `/api/generate`
- **JSON Body**: Model, prompt, and stream settings
- **Real-time Parsing**: Extracts "response" field from JSON stream

## Usage Examples

### Basic Usage
```cpp
String response = sendPromptStreamToOllama("192.168.1.100", 11434, "gemma:2b", "Hello!");
```

### Class-based Usage
```cpp
OllamaClient client("192.168.1.100", 11434);
String response = client.sendPromptStreamToOllama("gemma:2b", "How are you?");
```

## Response Processing
- Removes HTTP headers automatically
- Parses JSON responses in real-time
- Handles Unicode and special characters
- Supports both streaming and complete responses
```

## 📁 main.cpp

```markdown
# Main Controller - LED Test Program

## Overview
Simple test program for ESP32-S3 development board. Controls three LEDs for basic hardware verification.

## Hardware Setup
- **LED1**: GPIO12
- **LED2**: GPIO11  
- **LED3**: GPIO10

## Functionality
- Initializes all LED pins as outputs
- Creates blinking pattern on all LEDs
- 1-second ON, 1-second OFF cycle

## Purpose
- Verifies GPIO functionality
- Tests basic Arduino framework setup
- Confirms power and ground connections

## Code Structure
```cpp
void setup() {
    // Initialize LED pins
}

void loop() {
    // Turn all LEDs ON
    // Wait 1 second
    // Turn all LEDs OFF  
    // Wait 1 second
}
```

## Usage
This serves as a basic template for more complex applications. Replace the LED blinking logic with your main application code.
```

## 📁 oled_display.h

```markdown
# OLED Display - Visual Interface Manager

## Overview
Manages SSD1306 OLED display to show robot emotions and status through facial expressions.

## Display Specifications
- **Resolution**: 128x64 pixels
- **Interface**: I2C
- **Address**: 0x3C
- **Library**: Adafruit_SSD1306

## Key Functions

### Initialization
- `setup_display()` - Initializes OLED and verifies connection

### Facial Expressions
- `Face_Smile()` - Happy face with curved mouth
- `Face_Normal()` - Neutral face with straight mouth  
- `Face_Cry()` - Sad face with tears
- `Face_Sad()` - Sad face without tears
- `Face_Thinking()` - Neutral face with thought bubbles
- `Face_Speaking()` - Talking face with sound waves

## Drawing Elements
- **Face**: 60px diameter circle
- **Eyes**: 8px diameter circles at (52,28) and (76,28)
- **Mouths**: Various curved and straight lines
- **Effects**: Tears, thought bubbles, sound waves

## Usage Example
```cpp
setup_display();
Face_Smile();        // Show happy face
delay(2000);
Face_Thinking();     // Show thinking face
```

## Emotional States
- **Happy**: Smile + curved mouth
- **Neutral**: Straight mouth  
- **Sad**: Downward curved mouth
- **Crying**: Sad face + tear lines
- **Thinking**: Neutral + thought bubbles
- **Speaking**: Neutral + sound waves
```

## 📁 RGB.h

```markdown
# RGB LED Controller

## Overview
Manages NeoPixel RGB LED for status indication and visual feedback.

## Hardware Setup
- **Pin**: GPIO48
- **Type**: WS2812B NeoPixel
- **Quantity**: 1 LED
- **Brightness**: 50% (configurable)

## Key Functions

### Initialization
- `setup_RGB()` - Initializes NeoPixel strip and sets brightness

### Color Functions
- `Show_Red()` - Solid red
- `Show_Green()` - Solid green  
- `Show_Blue()` - Solid blue
- `Show_Yellow()` - Solid yellow
- `Show_Black()` - LED off
- `Show_White()` - Solid white

## Color Codes
- **Red**: (255, 0, 0) - Error/Stop
- **Green**: (0, 255, 0) - Ready/Success
- **Blue**: (0, 0, 255) - Processing
- **Yellow**: (255, 255, 0) - Warning
- **White**: (255, 255, 255) - Active
- **Black**: (0, 0, 0) - Off/Idle

## Usage Example
```cpp
setup_RGB();
Show_Blue();        // Show blue during processing
delay(1000);
Show_Green();       // Show green when ready
```

## Applications
- System status indication
- Voice processing feedback
- Network connection status
- Error state visualization
```

## 📁 robot_movement.h

```markdown
# Robot Movement Controller

## Overview
Controls L298N motor driver for 2-wheel robot movement. Provides forward, backward, and turning functions.

## Hardware Configuration
```
L298N Motor Driver:
  IN1 -> GPIO4    (Left motor forward)
  IN2 -> GPIO10   (Left motor backward) 
  IN3 -> GPIO11   (Right motor forward)
  IN4 -> GPIO12   (Right motor backward)
```

## Movement Functions

### Basic Movements
- `Move_Forward(duration)` - Both wheels forward
- `Move_Backward(duration)` - Both wheels backward
- `Move_Left(duration)` - Right wheel forward only
- `Move_Right(duration)` - Left wheel forward only

### Advanced Movements  
- `Turn_Left_InPlace(duration)` - Wheels in opposite directions
- `Turn_Right_InPlace(duration)` - Wheels in opposite directions

## Timing Control
- **Forward/Backward**: 300ms per duration unit
- **Turning**: 200ms per duration unit  
- **Automatic Stop**: Motors stop after movement completion

## Safety Features
- Automatic motor stopping after movement
- Constructor initializes motors to stopped state
- Serial logging of all movement commands

## Usage Example
```cpp
Robot myRobot;
myRobot.Move_Forward(2.0);    // Move forward for 0.6 seconds
myRobot.Move_Left(1.5);       // Turn left for 0.3 seconds
```

## Movement Patterns
- **Differential Steering**: Different wheel speeds for turning
- **Tank Steering**: Opposite wheel rotation for spot turns
- **Timed Control**: Duration-based movement control
```

## 📁 AI_Personality.py

```markdown
# AI Personality Configuration

## Overview
Defines character personalities and conversation rules for the AI chat companion.

## Personalities

### English Personality: "Sophia"
- **Age**: 20-year-old girl
- **Tone**: Warm, friendly, sweet, playful
- **User Context**: Lorenzy, 19, EE student at TU Delft
- **Interests**: Programming, AI, robotics

### Chinese Personality: "小雅" (Xiao Ya)  
- **Age**: 20-year-old young woman
- **Tone**: Warm, kind, relaxed, cute
- **User Context**: 小陈, 7 years old
- **Interests**: Guzheng, mathematics

## Conversation Rules

### Response Guidelines
- Maximum 2-3 sentences per response
- Natural, conversational tone
- No AI/robot/model references
- No user dialogue reproduction
- Light emotions and cute expressions

### Format Restrictions  
- No character name labels in responses
- Short, concise answers
- Friend-like texting style
- No rule explanations

## Usage
```python
from AI_Personality import basic_data_en, basic_data_cn

# English conversation
prompt_en = basic_data_en + user_message

# Chinese conversation  
prompt_cn = basic_data_cn + user_message
```

## Application
- Provides consistent AI personality
- Maintains character immersion
- Ensures appropriate response style
- Supports multilingual interactions
```

## 📁 cmd.py

```markdown
# Serial Command Controller

## Overview
Simple Python script for manual robot control via serial communication.

## Features
- Serial communication with ESP32
- Real-time robot control
- Simple command interface
- Cross-platform port configuration

## Command Mapping
```
f -> Move Forward
b -> Move Backward  
l -> Move Left
r -> Move Right
q -> Quit program
```

## Serial Configuration
- **Baud Rate**: 115200
- **Timeout**: None (blocking)
- **Port**: Configurable (COM11 in example)

## Platform Ports
- **Windows**: COM3, COM4, COM5, etc.
- **Linux**: /dev/ttyUSB0, /dev/ttyUSB1, etc.
- **Mac**: /dev/tty.usbserial-*

## Usage
```bash
python cmd.py
```

## Example Session
```
开始控制机器人！输入 f,b,l,r 控制移动，q退出
命令: f
发送: f
命令: l  
发送: l
命令: q
```

## Error Handling
- Invalid command detection
- Serial connection error reporting
- Graceful program termination
```

## 📁 llm.py

```markdown
# LLM Python Client

## Overview
Python implementation for communicating with Ollama LLM server with streaming response support.

## Configuration
- **Ollama Host**: http://192.168.178.91:11434
- **Default Model**: gemma:2b
- **Streaming**: Enabled by default

## Key Function
- `send_prompt_stream(prompt)` - Sends prompt and streams response tokens

## Features
- Real-time token streaming
- JSON response parsing
- Error handling for network issues
- Continuous output during generation

## HTTP Request
```json
{
  "model": "gemma:2b",
  "prompt": "user message",
  "stream": true
}
```

## Response Processing
- Iterates through streaming response lines
- Parses JSON objects for "response" field
- Prints tokens in real-time
- Accumulates full response

## Usage Example
```python
from llm import send_prompt_stream

response = send_prompt_stream("Hello, how are you?")
# Output streams tokens as they're generated
```

## Applications
- Standalone LLM testing
- Python-based AI integration
- Response streaming demonstration
- Model performance testing
```

## 📁 main.py

```markdown
# Main AI Chat Server

## Overview
Comprehensive voice AI system integrating speech recognition, LLM processing, and text-to-speech with robot control capabilities.

## System Architecture

### Core Components
- **Vosk STT**: Speech-to-text recognition
- **Ollama LLM**: Language model processing  
- **Edge-TTS**: Text-to-speech synthesis
- **Socket Server**: ESP32 communication
- **Robot Control**: Movement command handling

## Server Configuration
- **STT Port**: 5005 (Speech input)
- **TTS Port**: 5006 (Audio output)
- **Sample Rate**: 16kHz
- **Audio Format**: PCM16 LE

## Key Features

### Voice Control
- Natural language movement commands
- Real-time speech recognition
- Automatic command classification

### AI Personality
- Configurable character personas
- Bilingual support (EN/CN)
- Context-aware responses

### Audio Pipeline
- MP3 to WAV conversion
- Sample rate standardization
- Chunked audio streaming

## Control Commands
| Command | Keywords | Action |
|---------|----------|--------|
| Forward | "前进", "forward", "过来" | Move forward |
| Backward | "后退", "back" | Move backward |
| Left | "左边", "左转", "left" | Turn left |
| Right | "右边", "右转", "right" | Turn right |

## Communication Protocol

### STT Channel (5005)
- Receives raw audio data from ESP32
- Processes speech to text
- Triggers LLM/TTS pipeline

### TTS Channel (5006)  
- Sends control commands: `!CTRL#COMMAND#END`
- Sends audio pause commands: `!PAUSE_STT#DURATION#END`
- Streams PCM16 audio data

## Audio Processing
1. **STT**: Vosk processes 16kHz PCM to text
2. **LLM**: Ollama generates response with personality
3. **TTS**: Edge-TTS creates MP3, converts to WAV
4. **Streaming**: Chunked PCM16 sent to ESP32

## Usage
```bash
python main.py
```

## File Output
- Saves TTS MP3 and WAV files to Desktop/tts_output/
- Timestamped filenames for debugging
- Automatic directory creation

## Error Handling
- Connection loss recovery
- LLM timeout management
- Audio processing fallbacks
- Robot command verification
```

这些 README 文件详细说明了每个模块的功能、使用方法和集成方式。需要我对任何文件进行更详细的说明吗？