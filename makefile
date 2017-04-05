CXX=g++
INCDIR=include
CXXFLAGS=-I $(INCDIR) -c
LDFLAGS=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

SRCDIR=src
SOURCES=Aircraft.cpp Animation.cpp Application.cpp Button.cpp Command.cpp CommandQueue.cpp Component.cpp Container.cpp DataTables.cpp EmitterNode.cpp Entity.cpp GameOverState.cpp GameState.cpp Label.cpp Main.cpp MenuState.cpp MusicPlayer.cpp ParticleNode.cpp PauseState.cpp Pickup.cpp Player.cpp Projectile.cpp ResourceHolder.cpp SceneNode.cpp SettingsState.cpp SoundNode.cpp SoundPlayer.cpp SpriteNode.cpp State.cpp StateStack.cpp TextNode.cpp TitleState.cpp Utility.cpp World.cpp
SRCFILES=$(SOURCES:%.cpp=$(SRCDIR)/%.cpp)

OBJDIR=bin
OBJFILES=$(SOURCES:%.cpp=$(OBJDIR)/%.o)

EXEC=aircombat

all:$(EXEC)

$(EXEC):$(OBJFILES)
	$(CXX) $^ -o $@  $(LDFLAGS)

$(OBJFILES): $(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(OBJDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -rf bin $(EXEC)
