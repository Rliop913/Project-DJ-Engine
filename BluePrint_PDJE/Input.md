Treat input as a minimum delay.

Windows: Raw Input
Linux: [[EVDEV]]
MAC OS: IO KIT

There's an implementation of each one, and it supports the cross platform.

all applications are satisfied with update frames less than 1 ms.

Also, they all use the interactive method.


```mermaid
flowchart TD
PDJE_Input --> windows --> Raw_Input --> Regular_I
PDJE_Input --> RTMidi --> midi
PDJE_Input --> linux --> linevdev --> Regular_I
```

```mermaid
flowchart TD
input --> push --> _val_
input --> pull --> _val_
input --> move --> _val_
```

```mermaid
flowchart TD
interface --> search
interface --> set
interface --> get
interface --> pair_job

search --> devices
search --> memorized


devices --> role
devices --> name
devices --> type


set --> device
set --> type

get --> device
get --> type
get --> status

pair_job --> pair_input_to_function
pair_job --> unpair_function

```

