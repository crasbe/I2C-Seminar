#-----------------------------------------------------------------------
# I2C-Seminar, TUHH, SoSe 2017
# ----------------------------
# Makefile
#
# Autoren: Christopher Büchse und Jan Burmeister
#-----------------------------------------------------------------------
# Optionen:
#
# make                   = die Software kompilieren
# make clean             = das Projektverzeichnis aufräumen
# make doxygen           = Doxygen Dokumentation erzeugen
#
# Um das Projekt neu zu bauen, erst "make clean", dann "make" ausführen
#-----------------------------------------------------------------------

# Die Plattform spezifizieren
#    Mögliche Plattformen:
#       unix: Linux, MacOS X
#       win:  Windows 32- und 64-bit (ab Windows 7 getestet)
#    HINWEIS: MacOSX ist noch nicht unterstützt!
#    Die passende Plattform auskommentieren.
#PLATTFORM = unix
PLATTFORM = win

# Compiler auswählen
#     Der Quellcode ist für gcc geschrieben, andere Compiler sind
#     eventuell mit Anpassungen möglich.
#CC = gcc-6
#CC = gcc
CC = gcc.exe

# Compiler Flags
#     Die Compilerflags setzen den C-Standard auf C99 und aktivieren
#     (fast) alle Warnungen.
CFLAGS =  -std=c99
CFLAGS += -Wall -Wextra -Wpedantic
CFLGAS += -fdiagnostics-color=always

# Definition Flags
#     Die Definitionsflags verhalten sich wie Definitionen in einer
#     Header- oder Quelldatei, werden aber im Makefile gesetzt.
#     Hier werden sie zum Einschalten der erweiterten Debugausgaben
#     verwendet.
DFLAGS = -DDEBUG


# Name der Zieldatei
ZIEL = i2cseminar

#=======================================================================
# AB HIER SIND KEINE ÄNDERUNGEN MEHR NOTWENDIG!
#=======================================================================

# Liste der C-Quelldateien
SRC = $(ZIEL).c LCD_I2C.c ublox.c

# Plattformspezifische Einstellungen
#     - Befehle zum löschen, kopieren und Ordner erstellen auswählen.
#     - Korrekte Quelldatei zum Ansprechen der seriellen Schnittstelle
#       laden.
#     - Für Windows: korrekte Dateiendung
#
# Linux/MacOSX
ifeq ($(PLATTFORM),unix)
	# Programme auswählen
	LOESCH = rm -rf
	KOPIER = cp
	ORDNER = mkdir

	SRC += i2cusb/seriell_unix.c
	SRC += i2cusb/i2cusb.c
endif

# Windows
ifeq ($(PLATTFORM),win)
	LOESCH = del
	COPY = copy
	ORDNER = mkdir

	SRC += i2cusb\seriell_win.c
	SRC += i2cusb\i2cusb.c

	ENDUNG = .exe
endif


# Objektdateien als Abhängigkeit definieren
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
	$(CC) -c $< $(CFLAGS) $(DFLAGS) -o $@


# Projektverzeichnis saeubern
clean:
	@echo $(MSG_CLEAN)
	$(LOESCH) *.o $(ZIEL)$(ENDUNG)

# Programm ausfuehren
run: $(ZIEL)$(ENDUNG)
	./$(ZIEL)$(ENDUNG)

doxygen:
	doxygen Doxyfile
	$(MAKE) -C Doxygen/latex


.PHONY : all clean ccversion run
