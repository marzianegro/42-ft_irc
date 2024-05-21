# ft_irc

## Description
The `ft_irc` project at 42 is a networking project that involves creating an IRC (Internet Relay Chat) server. The goal is to implement a server that adheres to the IRC protocol, allowing multiple clients to connect, communicate in channels, and exchange messages in real-time.

## Features
- **IRC Protocol Compliance**: Implements the core functionalities of the IRC protocol.
- **Multi-client Support**: Allows multiple clients to connect and interact simultaneously.
- **Channels**: Supports the creation and management of chat channels.
- **User Commands**: Implements essential IRC commands such as JOIN, PART, PRIVMSG, and more.
- **Authentication**: Handles user registration and authentication.

## Getting Started
### Installation
1. Clone the repository:
    ```bash
    git clone https://github.com/marzianegro/42-ft_irc.git
    ```

2. Navigate to the project directory:
    ```bash
    cd ft_irc
    ```

3. Compile the project:
    ```bash
    make
    ```

### Usage
1. Start the IRC server with the desired port and password:

    ```bash
    ./ircserver <port> <password>
    ```

2. Connect to the server using an IRC client (e.g., `nc`):
    ```bash
    nc -C <port> <password>
    ```

3. Request a list of capabilities that the server supports:
    ```bash
    CAP LS 302
    ```

4. Authenticate with the server using the password provided:
    ```bash
    PASS <password>
    ```

5. Set your username with the `USER` command:
    ```bash
    USER <username> 0 * :realname
    ```

6. Set your nickname with the `NICK` command:
    ```bash
    NICK <nickname>
    ```

### Example Commands

- **Join a Channel**:
    ```bash
    JOIN #channelname
    ```

- **Leave a Channel**:
    ```bash
    PART #channelname
    ```

- **Send a Private Message**:
    ```bash
    PRIVMSG <username> <message>
    ```

## Contributors

- [mnegro](https://github.com/marzianegro)
- [ggiannit](https://github.com/skyheis)

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
