# Protocol & Resources

## What is a protocol

A protocol is an agreed set of technical rules for how two programs talk
to each other: what bytes to send, in what order, and what each side does
in response.
 
The internet runs almost entirely on protocols, each handling one specific 
function: routing packets, resolving names,
transferring web pages, delivering mail. 
**IRC** is no different. It's defined by its own RFCs
 
*RFC — Request for Comments.*
 
| Protocol | Defining RFC                  | Port        | Purpose                                           |
|----------|-------------------------------|-------------|----------------------------------------------------|
| IP       | RFC 791                       | —           | Routes packets between hosts                      |
| TCP      | RFC 9293 (obsoletes RFC 793)  | —           | Reliable, ordered byte stream over IP             |
| DNS      | RFC 1035                      | 53          | Resolves domain names to IP addresses             |
| HTTP     | RFC 9110 (obsoletes RFC 2616) | 80 / 443    | Transfers web resources between client and server |
| SMTP     | RFC 5321 (obsoletes RFC 821)  | 25          | Transfers mail between servers                    |
| IRC      | RFC 2812                      | 6667 / 6697 | Real-time text chat between client and server     |
 
A protocol only works if it's written down precisely enough that two
programmers, working independently, can each implement it and still
interoperate. That written record is what the **IETF** (Internet
Engineering Task Force) produces, as numbered, versioned documents
called **RFCs**.

### Why IRC has four of them
 
The original 1993 document (RFC 1459) tried to cover everything in one
file: architecture, client commands, channels, server linking. 
In 2000 it was split into four focused RFCs, each owning one slice:
 
- **RFC 2810 — Architecture**: how the network itself is shaped (servers, topology)
- **RFC 2811 — Channel Management**: channels, their modes, and operator privileges
- **RFC 2812 — Client Protocol**: the client↔server conversation
- **RFC 2813 — Server Protocol**: the server↔server conversation
 
## Official protocol (must read)
 
- [RFC 2812 — IRC: Client Protocol](https://www.rfc-editor.org/info/rfc2812/): the IETF RFC that actually defines client-server communication: message format, every command, and the numeric replies they trigger.
- [RFC 2811 — IRC: Channel Management](https://www.rfc-editor.org/info/rfc2811/): defines how channels work: creation, the `i t k o l` modes (invite-only, topic-protected, key, operator, limit), and operator privileges.

## Unofficial companion
 
- [modern.ircdocs.horse](https://modern.ircdocs.horse/): not an RFC, not an official document.
An independent, community-maintained rewrite of the same material. Useful as a secondary reference because it's organized more usably (numerics listed right under each command) and flags what's RFC-era core vs. modern IRCv3 extras.

## Background — historical
 
- [RFC 1459 — Internet Relay Chat Protocol](https://www.rfc-editor.org/info/rfc1459/): the original 1993 IETF document, published "Experimental" status. Written as one single document covering architecture, client commands, and server linking together, before being split and clarified into four separate RFCs (2810–2813) in 2000. 
