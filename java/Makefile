JFLAGS =
JC = javac
RM = rm
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = src/org/adrianwalker/lotto/Lotto.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) src/org/adrianwalker/lotto/Lotto.class
