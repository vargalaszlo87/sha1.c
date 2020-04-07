# sha1.c
This is a simple SHA1 hash generator in pure C without external library.

## Notice
Workd with 7bit ASCII, does not support UTF-8.

## Dependencies
The sha1ASCII7bit function needs some other functions. (Contained in sha1.c file.)
```C
void push(int item);
int pop();
int rotate_left(a, b);
const char * toHexString(int num);
const char * toHex(int num);
```

## Usage
It's very simply:
```C
char *hash = sha1ASCII7bit(msg);
```

## Example
```C
int main(int argc, char *argv[]) {
	char *msg = "hittheroadjack";
	char *hash = sha1ASCII7bit(msg);
	printf ("SHA1-Convert (%s): %s",msg,hash);	
	return 0;
}
```

## License
The project is released under the [MIT license](http://www.opensource.org/licenses/MIT).

## Contact
The project's website is located at https://github.com/vargalaszlo87/sha1.c

My personal website is located at http://vargalaszlo.com/index.php?q=c_sha1

Author: Laszlo, Varga (varga.laszlo.drz@gmail.com)

