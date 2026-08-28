# AsconJAS-NFC-IoT

Research repository containing the source code and experimental
materials for the evaluation of lightweight authenticated
encryption in NFC-enabled IoT environments.

## Project Overview

This repository contains the implementations, experimental
applications, analysis scripts, and configuration files used
in the research testbed.

## Implementations

The repository includes:

- Ascon reference implementation
- Optimised Ascon implementation
- Table-based Ascon variant
- NFC communication components
- Trust and cryptographic integration components

## Experimental Applications

Two experimental application configurations are provided:

- akhenaton
- nefertiti

Both applications use the optimised Ascon implementation
through `asconopt.c`.

## Analysis

The repository includes scripts for:

- node analysis
- CSV analysis
- RAM analysis
- trace analysis
- performance metrics extraction
- flamegraph generation

## Reproducibility

The source code and configuration files are provided to support
reproduction of the experimental evaluation.

Generated build files, binaries, object files and Python virtual
environments are excluded from the repository.

## Status

Research software associated with the NFC-enabled IoT
cryptographic evaluation.
