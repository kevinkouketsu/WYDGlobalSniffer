# WYD Global Message Sniffer

This project is a simple sniffer designed to be injected into the **WYD Global** game client. It hooks into the `CPSock` class, specifically the `ReadMessage` and `AddMessage` methods, to intercept messages exchanged between the client and the server.

## Usage

**Note:** This tool is meant for educational and debugging purposes.

1. Compile the DLL project.
2. Inject the DLL into the WYD Global client process using your preferred injector.
3. Watch the console output to see intercepted messages.

## Disclaimer

This software is provided **"as is"**, without warranty of any kind.  
The author is **not responsible** for any damage, account bans, or misuse caused by this project.  
Use it **at your own risk**, and only in accordance with local laws and the terms of service of the software you are analyzing.

## License

MIT License — see [LICENSE](./LICENSE) for details.
