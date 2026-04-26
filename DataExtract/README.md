# DataExtract

This directory contains local analysis scripts and generated artifacts used to
inspect runtime or benchmark output. It is not part of the core engine
architecture or the canonical documentation set.

Typical contents:

- raw text captures such as `fullManualRes.txt` and `hybridRes.txt`
- CSV extraction helpers such as `csv_extract.py`
- visualization helpers such as `visualize_csv.py`
- generated plots and summary images

Use this directory when you need to inspect performance or callback-shape data.
Do not treat it as the source of truth for runtime APIs, build defaults, or
platform support. Canonical repository control docs live under `../AGENT_DOCS/`.
