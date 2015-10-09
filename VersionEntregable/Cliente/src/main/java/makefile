JFLAGS = -g
JC = javac
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = \
	ThreadCliente.java \
	Chat.java \
	Cliente.java \
	main.java 

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) *.class
