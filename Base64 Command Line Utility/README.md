# Base64-CLI

This is a Base64 utility you can use in your command line. The tool has an input source, and output destination. Regardless of the source, the process is the same. The source is read, encoded, and then delivered to the output source. There are multiple input sources you can use. If you don't specify an input source, it will use stdin as an input source. The input sources are `f=file.txt` for a file and `t=HelloWorld` for text. Make sure to surround the whole input source in quotes if it contains a space, i.e. `"t=Hello World"`. As for output sources, if you don't specify one with `-o=output.bin`, then it will simply put it through to `stdout`.

An input mode must be specified, this can be either `-e` or `-d` for encoding and decoding respectively.

### Example Usages
```
base64 -f=image.png -o=image.b64 -e
``` 
```
base64 -f=image.b64 -o=image.png -d
```
```
base64 "-t=Hello World" -o=message.txt -e
```
```
base64 -t=aGVsbG8gd29ybGQ= -d
```
