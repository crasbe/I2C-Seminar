#-----------------------------------------------------------------------
# I2C-Seminar, TUHH, SoSe 2017
# ----------------------------
# Makefile
# 
# Autoren: Christopher Buechse und Jan Burmeister
#-----------------------------------------------------------------------
# Optionen:
#
# make                   = die Software kompilieren
# make clean             = das Projektverzeichnis aufräumen
#
# Um das Projekt neu zu bauen, erst "make clean", dann "make" ausfuehren
#-----------------------------------------------------------------------

# Die Plattform spezifizieren
#    Moegliche Plattformen sind linux, Windows 32- und 64-bit, sowie
#    MacOSX.
#    HINWEIS: Windows und MacOSX sind noch nicht unterstuetzt!
#    Die passende Plattform auskommentieren.
PLATTFORM = linux
#PLATTFORM = win32
#PLATTFORM = win64
#PLATTFORM = macosx

# Name der Zieldatei
ZIEL = i2cseminar

# Liste der C-Quelldateien
SRC = $(ZIEL).c i2cusb.c

# Plattformspezifische Definitionen von Variablen
#     - Die korrekte Quelldatei zum Ansprechen der
#       seriellen Schnittstelle laden.
#     - Fuer Windows die Dateiendung .exe spezifizieren
ifeq ($(PLATTFORM),linux)
    SRC += seriell_linux.c
endif
ifeq ($(PLATTFORM),win32)
    SRC += seriell_win32.c
    ENDUNG = .exe
endif
ifeq ($(PLATTFORM),win64)
    SRC += seriell_win64.c
    ENDUNG = .exe
endif
ifeq ($(PLATTFORM),macosx)
    SRC += seriell_macosx.c
endif

# Compiler Flags
#     Die Compilerflags setzen den C-Standard auf C99 und aktivieren
#     (fast) alle Warnungen.
CFLAGS =  -std=c99
CFLAGS += -Wall -Wextra -Wpedantic
CFLAGS += -DDEBUG

#=======================================================================
# Programme auswaehlen

LOESCH = rm -rf
KOPIER = cp
ORDNER = mkdir

# Compiler auswaehlen
#     Der Quellcode ist fuer gcc geschrieben, andere Compiler sind
#     eventuell mit Anpassungen moeglich.
CC = gcc-6
#CC = gcc

# Objektdateien als Abhaengigkeit definieren
OBJ = $(SRC:.c=.o)

# Shellausgaben
MSG_CLEAN = Sauebere Projekt...
MSG_LINK = Linke Dateien:
MSG_COMPILE = Kompiliere:


# Standard-Target
all: ccversion $(ZIEL)


# Compiler-Version ausgeben
ccversion:
	$(CC) --version
    
# Target linken
$(ZIEL): $(OBJ)
	@echo $(MSG_LINK) $@
	$(CC) $(LDFLAGS) $^ --output $@$(ENDUNG)
   
# Target kompilieren
%.o: %.c
	@echo $(MSG_COMPILE) $<
	$(CC) -c $(CFLAGS) $< -o $@
    

# Projektverzeichnis saeubern
clean:
	@echo $(MSG_CLEAN)
	$(LOESCH) *.o $(ZIEL)$(ENDUNG)
    
# Programm ausfuehren
run: $(ZIEL)$(ENDUNG)
	./$(ZIEL)$(ENDUNG)
	
.PHONY : all clean ccversion run
