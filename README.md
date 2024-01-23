# Home security
## Idea
1. Use old phones as security camera for the house
2. Use camera stream to run object detection to look for poisonous animals (mainly snake)
## TODOS
1. Build a sever to receive a camera stream over udp ✅
2. Build a phone app to send camera stream on demand
3. Train an AI model for inferencing
4. Use a radar kit for movement detection (snakes cannot be captured on PIR sensors, they are cold blooded)
5. Send radar signals to the server
6. Based on radar signals ask the udp clients (phones) to send camera stream
## Architecture
![Architecture](architecture.excalidraw.png)