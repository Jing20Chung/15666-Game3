.PHONY : all

#n.b. the '-y' sets autoexec scripts to 'on' so that driver expressions will work
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	BLENDER = /Applications/Blender.app/Contents/MacOS/Blender -y
else ifeq ($(UNAME_S),Linux)
	BLENDER = ../../blender-4.5.2-linux-x64/blender -y
else
	BLENDER = blender
endif

PYTHON := /opt/homebrew/bin/python3

EXPORT_MESHES=export-meshes.py
EXPORT_SCENE=export-scene.py
EXPORT_LEVEL=export-level.py

DIST=../dist

all : \
	$(DIST)/game3_1.pnct \
	$(DIST)/game3_1.scene \
	$(DIST)/game_win.pnct \
	$(DIST)/game_win.scene \
	$(DIST)/game_lose.pnct \
	$(DIST)/game_lose.scene \
	$(DIST)/game3_1.level \


$(DIST)/game3_1.scene : game3_1.blend $(EXPORT_SCENE)
	$(BLENDER) --background --python $(EXPORT_SCENE) -- '$<':Main '$@'

$(DIST)/game3_1.pnct : game3_1.blend $(EXPORT_MESHES)
	$(BLENDER) --background --python $(EXPORT_MESHES) -- '$<':Main '$@'

$(DIST)/game_win.scene : game_win.blend $(EXPORT_SCENE)
	$(BLENDER) --background --python $(EXPORT_SCENE) -- '$<':Main '$@'

$(DIST)/game_win.pnct : game_win.blend $(EXPORT_MESHES)
	$(BLENDER) --background --python $(EXPORT_MESHES) -- '$<':Main '$@'

$(DIST)/game_lose.scene : game_lose.blend $(EXPORT_SCENE)
	$(BLENDER) --background --python $(EXPORT_SCENE) -- '$<':Main '$@'

$(DIST)/game_lose.pnct : game_lose.blend $(EXPORT_MESHES)
	$(BLENDER) --background --python $(EXPORT_MESHES) -- '$<':Main '$@'

$(DIST)/game3_1.level : Level_1_Map.txt $(EXPORT_LEVEL)
	$(PYTHON) $(EXPORT_LEVEL) -- '$<' '$@'