# Markov Chain Text Generator

Generates sentences utilizing Markov chains given an input of text.

## How it Works

Given a text file, the program first splits the text into words. By utilizing a hash table and iterating over each word, the program constructs a graph associating each word to the word following it, this works by having a list of words associated with one key.

Generating sentences from this graph becomes quite easy. At first the program selects a random key from the hash table and the associated list of words and then randomly selects a word from that list, writing it to the file, and then that word becomes the new key. This repeats for a given number of words and sentences defined by the user.

## Building

Run:

```console
$ make
```

## Running

The program accepts 4 arguments in this order:

- The file containing the text to be read from.
- The number of words per sentence.
- The number of sentences.
- File to write the generated sentences to. (optional)

### Example

```console
$ ./release/main test.txt 10 5 result.txt
```

## About the Source Code

This was tested and complied using gcc version 13.1.0.

## Libraries Used

- [String_View By Tsoding](https://github.com/tsoding/sv)
- [stb_ds.h by Nothings](https://github.com/nothings/stb/blob/master/stb_ds.h)
