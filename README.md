This project contains solutions for crypto project of School 21.
These project require implementing some popular crypto functions along with openssl-like CLI.
I've implemented following crypto functions:
* MD5
* SHA256
* Base64 (not a crypto function)
* DES


## Usage

``` bash
make
```

### MD5

``` bash
./build/ft_ssl md5 -s "hello there"
MD5 ("hello there") = 161bc25962da8fed6d2f59922fb642aa
```

### SHA256

``` bash
./build/ft_ssl sha256 -s "hello there"
SHA256 ("hello there") = 12998c017066eb0d2a70b94e6ed3192985855ce390f321bbdb832022888bd251
```

### Base64

``` bash
echo "hello there" | ./build/ft_ssl base64 -e
aGVsbG8gdGhlcmUK
```

### DES

``` bash
echo "hello there" | ./build/ft_ssl des -k 123 -v 123 -a
4wCm5pkq1ChSYalPTW072g==
```

## Resources

* [MD5 wiki](https://ru.wikipedia.org/wiki/MD5)
* [MD5 RFC](https://www.rfc-editor.org/rfc/rfc1321)
* [SHA2 wiki](https://en.wikipedia.org/wiki/SHA-2)
* [SHA2 lecture](https://www.youtube.com/watch?v=JIhZWgJA-9o&list=WL&index=48&t=3117s)
* [SHA RFC](https://www.rfc-editor.org/rfc/rfc6234)
* [Good implementations of crypto algorithms](https://github.com/B-Con/crypto-algorithms)
* [Base64 wiki](https://ru.wikipedia.org/wiki/Base64)
* [Base64 video](https://www.youtube.com/watch?v=aUdKd0IFl34)
* [DES wiki](https://ru.wikipedia.org/wiki/DES)
* [Reference DES implementation](https://github.com/dhuertas/DES/blob/master/des.c)
* [Several details about DES](https://rucore.net/algoritm-shifrovaniya-des/)