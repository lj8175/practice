all:linux rtems

linux:pthreadsignal.c
	gcc $< -o $@ -lpthread

rtems: rtems_wrap.o pthreadsignal.o
	PATH=/opt/rtems-4.10/bin:${PATH} /opt/rtems-4.10/libexec/gcc/i386-rtems4.10/4.4.5/collect2 -dc -dp -N /home/rtems/rtems-4.10-work/bsp-install/i386-rtems4.10/pc386/lib/start.o /opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5/crti.o /opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5/crtbegin.o -e start -L/home/rtems/rtems-4.10-work/bsp-install/i386-rtems4.10/pc386/lib -L/opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5 -L/opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5/../../../../i386-rtems4.10/lib -Ttext 0x00100000 $^ -lgcc --start-group -lrtemsbsp -lrtemscpu -lc -lgcc --end-group -T /home/rtems/rtems-4.10-work/bsp-install/i386-rtems4.10/pc386/lib/linkcmds -lgcc /opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5/crtend.o /opt/rtems-4.10/lib/gcc/i386-rtems4.10/4.4.5/crtn.o -o $@

rtems_wrap.o: rtems_wrap.c
	/opt/rtems-4.10/bin/i386-rtems4.10-gcc -c $< -B/home/rtems/rtems-4.10-work/bsp-install/i386-rtems4.10/pc386/lib/ -specs bsp_specs -qrtems -g -Wall -g -mtune=i386 -Wl,-Ttext,0x00100000 -o $@

pthreadsignal.o: pthreadsignal.c
	/opt/rtems-4.10/bin/i386-rtems4.10-gcc -c $< -B/home/rtems/rtems-4.10-work/bsp-install/i386-rtems4.10/pc386/lib/ -specs bsp_specs -qrtems -g -Wall -g -mtune=i386 -Wl,-Ttext,0x00100000 -o $@

clean:
	rm rtems
	rm linux
	rm rtems_wrap.o
	rm pthreadsignal.o
