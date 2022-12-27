This project contains solutions for crypto project of School 21.
For now I've implemented several hashing function here.

## Usage

``` bash
make
```

Passing input via pipe:
``` bash
echo 'hello there' | ./build/ft_ssl
```

Passing input as string:
``` bash
./build/ft_ssl -s 'hello there'
```

Passing input from file:
``` bash
echo 'hello there' > hello_there.txt
./build/ft_ssl hello_there.txt
```

## Resources

* [MD5 wiki](https://ru.wikipedia.org/wiki/MD5)
* [MD5 RFC](https://www.rfc-editor.org/rfc/rfc1321)
* [SHA2 wiki](https://en.wikipedia.org/wiki/SHA-2)
* [SHA2 lecture](https://www.youtube.com/watch?v=JIhZWgJA-9o&list=WL&index=48&t=3117s)
* [SHA RFC](https://www.rfc-editor.org/rfc/rfc6234)
* [Good implementations of crypto algorithms](https://github.com/B-Con/crypto-algorithms)