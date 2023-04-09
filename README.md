<br/>
<h2 align="center">⚙️ just the <b>gxen</b> game engine <b>render</b> module ⚙️</h2>

<p align="center">
	<img src="docs/infinity_grid.gif" />
</p>
<br/>

> <code><b>[Warning]</b> The project is under development.
> New versions could be unstable and non-backward-compatible.</code>

<h2>📜 Table of contents </h2>

- [🧟 Cloning and building](#-cloning-and-building)
- [🧱 Project structure](#-project-structure)
	- [Dependencies](#dependencies)
- [⚖️ Licensing](#️-licensing)

## 🧟 Cloning and building

Clone the repo:

```sh
git clone --recurse-submodules -j8 https://github.com/golxzn/render.git
```

You could configure the project using CMake presets.

```sh
cmake --preset dev-windows
```

## 🧱 Project structure

### Dependencies

- CMake 3.23 or higher;
- OpenGL;
- [glm](https://github.com/g-truc/glm);
- [golxzn::core](https://github.com/golxzn/core).

## ⚖️ Licensing

See the [LICENSE](LICENSE) document.

<details>
<summary>File content:</summary>

```
MIT License

Copyright (c) 2022 Ruslan Golovinskii

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
</details>