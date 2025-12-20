# libunicode-conv
A lightweight library for [unicode encodings](https://en.wikipedia.org/wiki/Character_encoding) made in C.

libunicode-conv is like another libraries but with minimal dependencies and made to have a minimal footprint in runtime.
It is intended use is for internal representation of characters in programs, allowing for quick conversions between encondings.

Also, it is being developed to allow verbose error messages for quick debugging.

## Getting started and Installation

Clone the repo
```sh
git clone https://github.com/JavierNIS/libunicode-conv.git
```

At the moment, there are only tests, no libraries are made, but the compilation process is straight forward using the provided makefile. To compile the tests:

```sh
make
```

If you want to delete unit tests, it is as simple as running the following:

```sh
make clean
```

## Usage

For the tests:
1. UTF8ToUTF16 and UTF32conversionsTo: Just run the tests with:

```sh
./UTF8ToUTF16 [BE]
./UTF32conversionsTo [BE]
```

2. InputCharacter and UTF8stringsConversions:

```sh
./UTF8stringsConversions [BE]
./Inputcharacter [BE]
```

Use BE as a parameter if you want to use big endian

## Roadmap
- [ ] Add tests for incorrect input and add code to handle malformed codepoints.
- [ ] Add scripts to generate code based on the Unicode standard.
- [ ] Add normalization functions based on the generated code.
- [ ] Add tests for normalization functions.

## License

This README is distributed under the GNU LGPLv3. See `LICENSE` for more information. The whole repository is also governed under this License
