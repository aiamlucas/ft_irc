# Coding Style Guide

## 1. Naming
| What                         | Convention              | Example                               |
|------------------------------|-------------------------|---------------------------------------|
| Classes / types              | `PascalCase`            | `Client`, `Channel`, `CommandHandler` |
| Methods (public or private)  | `camelCase()`           | `handleJoin()`, `isOperator()`        |
| Private / protected members  | `_camelCase`            | `_nickname`, `_clients`               |
| Local variables & parameters | `camelCase`             | `targetChannel`, `rawLine`            |
| File names                   | match the class exactly | `Client.hpp` / `Client.cpp`           |

One class per header/source pair. If a file doesn't map to exactly one
class (e.g. free-function numeric-reply helpers), name it for what it
groups: `Replies.hpp`, `Parser.hpp`.

One class per header/source pair
`Client.hpp` / `Client.cpp`.


## 2. Formatting — `.clang-format`

```yaml
BasedOnStyle: Google
IndentWidth: 4
UseTab: Never
PointerAlignment: Left      # int* ptr — attached to the type, not the name
AccessModifierOffset: -4    # keeps private:/public: at column 0 with a 4-space IndentWidth
```
