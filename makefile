CFLAGS=-W -Wall -O6 #compilation : creation de .o
LDFLAGS=-L/usr/lib/arm-linux-gnueabihf -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lfreenect_sync #operation de linkage 

install_all:
	sudo apt-get update
	sudo apt-get install libsdl2-dev
	sudo apt-get install libsdl2-mixer-dev
	sudo apt-get install libsdl2-ttf-dev
	sudo apt-get install libsdl2-image-dev
	sudo apt-get install freenect
	sudo apt-get install libusb-1.0-0-dev
	make SlotTrack.exe

SlotTrack.exe: Collision.o FonctionsGenerales.o GestionEvent.o GestionInstruction.o GestionMenus.o GestionMusique.o GestionVoiture.o InitialisationEtFin.o JeuCerise.o JeuCourse.o JeuPriseFlag.o main.o Modelisation.o ObtenirCouleur.o traitement.o calibrage.o matrice.o acquisition.o
	gcc $^ -o SlotTrack.exe $(CFLAGS) $(LDFLAGS) -lm

%.o: %.c 
	gcc -c $^ $(CFLAGS) -lm

clean:
	rm -rf *.o
