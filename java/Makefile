JFLAGS =
JC = javac
RM = rm
.SUFFIXES: .java .class
.java.class:
	$(JC) $(JFLAGS) $*.java

CLASSES = src/org/adrianwalker/lotto/Lotto1.java\
  src/org/adrianwalker/lotto/Lotto2.java\
  src/org/adrianwalker/lotto/Lotto3.java

default: classes

classes: $(CLASSES:.java=.class)

clean:
	$(RM) src/org/adrianwalker/lotto/Lotto1.class
	$(RM) src/org/adrianwalker/lotto/Lotto2.class
	$(RM) src/org/adrianwalker/lotto/Lotto3.class	
