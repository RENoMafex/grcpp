<!--
This file is part of grcpp, a C++ rewrite of Radovan Garabiks 'grc'
Copyright (C) 2025 Malte Schilling schilling.malte@googlemail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
-->

# grcpp

#### This repository is a C++ rewrite of the tool [**grc**](https://github.com/garabik/grc) by Radovan Garabik.

[![Unit Tests](https://github.com/RENoMafex/grcpp/actions/workflows/unit_tests.yml/badge.svg)](https://github.com/RENoMafex/grcpp/actions/workflows/unit_tests.yml) [![System Tests](https://github.com/RENoMafex/grcpp/actions/workflows/system_tests.yml/badge.svg)](https://github.com/RENoMafex/grcpp/actions/workflows/system_tests.yml)

>[!WARNING]
>This software is NOT in a working state and should not be used. Please only use this code for educational or development purposes!

## Table of Contents
+ [1. About](#1-about)
+ [2. Changes](#2-changes)
  + [2.1 Details](#21-details)
  + [2.2 Why](#22-why)
+ [3. How to Contribute](#3-how-to-contribute)
+ [4. Features](#4-features)
+ [5. Installation](#5-installation)
+ [6. Usage](#6-usage)
+ [7. Acknowledgements](#7-acknowledgments#)
+ [8. Licenses](#8-licenses)
  + [8.1 Used librarys under different licenses](#81-used-librarys-under-different-licenses)

## 1. About

[**grc** (Generic Colouriser)](https://github.com/garabik/grc) is a tool for adding color to command-line output. This project, **grcpp**, aims to provide a modern, efficient, and extensible implementation, while still being able to be a drop-in replacement of the original tool using C++.

## 2. Changes

Grcpp aims to be faster than the original grc, that is why i decided to go for a single binary (opposed to grc wich uses another program called grcat to colorize the output) and utilize multithreading.

### 2.1 Details
#### Visual representation of how grc works
![flowchart of how grcpp works](.github/images/flowchartgrc.svg)

#### Conclusion
Now it should be pretty clear, that **grc** only is a wrapper for the underlying grcat (also part of the grc project) which can induce some performance overhead, so i decided to build this program into a single file, which simply utilizes threads to colorize the output(s).

### 2.2 Why?

My daily machine is a very old laptop where grc sometimes induces some noticable lag, i think for the most machines grc is just fine, but for me personally its to slow, thats why i decided to rebuild it in C++ in a performance optimized way.

## 3. How to contribute

- If you find any issue with the code, please just open an issue through GitHub. If you don't have a GitHub Account (and don't want to create one), just send an email to [schilling.malte@googlemail.com](mailto:schilling.malte@googlemail.com).
- If you want to contribute code to this repo, just open a Pull Request through GitHub.
>[!NOTE]
>Proper instructions for contributing to grcpp will be added later.

## 4. Features

- Drop-in replacement for **grc**.
- Improved performance and maintainability with modern C++.

## 5. Installation

A Makefile is included.
>[!NOTE]
>Proper instructions for building and installing grcpp will be added soon.

## 6. Usage

Usage examples and documentation will be provided as the project progresses.
When the project enters beta state, the usage will be the same as grc.

## 7. Acknowledgments

Special thanks to [Radovan Garabík](https://github.com/garabik) for creating the original **grc** tool.

## 8. Licenses

This project is licensed under the terms of the [GNU General Public License v3.0](http://www.gnu.org/licenses/gpl-3.0.html) or later.</br>
The original **grc** project is licensed unter the [GNU General Public License v2.0](http://www.gnu.org/licenses/gpl-2.0.html) or later.</br>

This project uses third party librarys, which are under different licenses. Details are in the directory `THIRD_PARTY_LICENSES/`.

### 8.1 Used librarys under different licenses

- Catch2 -- [Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt)
- Boost librarys -- [Boost Software License 1.0](https://www.boost.org/LICENSE_1_0.txt)

