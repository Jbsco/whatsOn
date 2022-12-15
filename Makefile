EXEC=whatsOn-v1.1

# INCLUDES=

${EXEC}: ${EXEC}.cpp Makefile
	g++ -g ${EXEC}.cpp -o ${EXEC} -Wall

run: ${EXEC}
	 ./${EXEC}

debug: ${EXEC}
	gdb ${EXEC}

clean:
	rm ${EXEC}