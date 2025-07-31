[[PDJE]] - The Rhythm Game Engine + DJ 
	[[Engines]]
	[[Interface]]
		[[DB]]
			[[Music]]
			 [[Track]]
		[[Audio_Render]]
			[[FullPreRender]]
			[[Hybrid Render]]
			[[FullManualRender]]
			[[EffectRender]]
		[[Editor]]
		[[Note_Render]]
		[[Input]]
		


Documentation
```mermaid
flowchart LR


index --> PDJ_modules
index --> Basic_usage
index --> PDJE_data_format
index --> greetings

PDJ_modules --> core
PDJ_modules --> input
PDJ_modules --> note
PDJ_modules --> util

core --> audio_renderer
core --> db
core --> editor

input --> general
input --> midi



```

```mermaid
flowchart LR

PDJE --> PDJ_Modules

PDJ_Modules --> PDJ_Core_Engine
PDJ_Modules --> PDJ_Input_Engine
PDJ_Modules --> PDJ_Note_Engine
PDJ_Modules --> PDJ_Utility_Engine


PDJ_Core_Engine --> PDJE_Editor
PDJ_Core_Engine --> PDJE_Audio_Renderer
PDJ_Core_Engine --> PDJE_DB

PDJ_Input_Engine --> general_input
PDJ_Input_Engine --> midi_input

```
