## ft_irc

### About

IRC server made with RFC 1459 standard and Pidgin chat client as reference.

Internet Chat Relay (IRC) - is designed for group communication in discussion forums, called channels, but also allows one-on-one communication via private messages as well as chat.

It offers real-time messaging that can be either public or private. Users can exchange
direct messages and join group channels.
IRC clients connect to IRC servers in order to join channels.

Build with C++98 standard library.

### Commands

* User:
  * PASS :\<password> - login into password protected server
  * USER \<username> \<hostname> \<servername> :\<realname> - provide the username, hostname, servername and realname of the new user
  * NICK \<nickname> - register with a nickname
  * JOIN \<channel> - join to chat
    * The user must be invited if the channel is invite-only
  * PART \<channel> - leave from chat
  * PRIVMSG \<nickname/channel> :\<text> - send message to user or channel
  * NOTICE \<nickname/channel> :\<text> - send message to user or channel without automatic reply
* Channel operator:
  * KICK \<channel> \<user> - eject a client from the channel
  * MODE \<channel> {[+|-]|i|t} [\<user>] - change the channel's mode
    * i - invite only
    * t - topic can be changed only by operator
  * INVITE \<nickname> \<channel> - invite a client to an invite-only channel
  * TOPIC \<channel> [\<topic>] - change the channel topic in a mode +t channel
    * The user must be operator if the channel in +t mode

### Requirements & usage
Project requires:
* C++ compiler: **gcc** or **clang**
* Possibility to work with Makefile

Tested on:
* Linux Ubuntu 21.10

Run:
```
$> make
$> ./ircserv [port] [password]
...
```

Usage:
* Pidgin chat client with GUI (select IRC protocol)
* With nc :
  * ```
    $> nc [IP] [PORT]
    $> ./ircserv [port] [password]
    ...
    ```

### Controls

* W/A/S/D - Move
* Q/E - Rotate
* Z/X - Scaling
* F/G - Shift Heights
* I/P - Change projection
* ESC - Exit