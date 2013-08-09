
javac HelloWorld.java
javah -jni HelloWorld
gcc -I/usr/lib/jvm/java-7-openjdk-i386/include -fPIC -shared -o libHelloWorld.so HelloWorld.c
java -Djava.library.path="." HelloWorld

