gcc -c foo.c
gcc -shared -Wl -o libfoo.so  foo.o
go run hello.go

rm libfoo.so  foo.o
