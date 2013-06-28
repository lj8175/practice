all:numberof1 count1

numberof1:numberof1.c
	gcc $< -o $@ -g

count1:count1.c
	gcc $< -o $@ -g

r:
	./count1 | tee count1.scn

clean:
	rm numberof1
	rm count1
