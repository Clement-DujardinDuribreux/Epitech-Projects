##
## EPITECH PROJECT, 2026
## Raytracer
## File description:
## Makefile
##

NAME =      raytracer

CXX =       g++
CXXFLAGS =  -std=c++20 -Wall -Wextra -Werror -fexceptions
CPPFLAGS =  -Isrc \
            -Isrc/interfaces \
            -Isrc/abstracts \
            -Isrc/math \
            -Isrc/primitives \
            -Isrc/lights \
            -Isrc/materials \
            -Isrc/raytracing \
            -Isrc/parser \
            -Isrc/optimisation \
            -Isrc/transforms \
            -Isrc/rendering \
            -I/opt/homebrew/include

LDFLAGS =   -L/opt/homebrew/lib -lconfig++

SRC = src/main.cpp \
      src/parser/ConfigParser.cpp \

SRC_PLUGINS =     src/primitives/Sphere.cpp \
                  src/primitives/Plane.cpp \
                  src/primitives/Cylinder.cpp \
                  src/primitives/Cone.cpp \
                  src/primitives/Triangle.cpp \
                  src/primitives/MobiusStrip.cpp \
                  src/primitives/Box.cpp \
                  src/primitives/Tetrahedron.cpp \
                  src/primitives/SierpinskiFractal.cpp \
                  src/primitives/Pyramide.cpp \
                  src/primitives/OBJ.cpp \
                  src/primitives/Box.cpp \
                  src/materials/ColorMaterial.cpp \
                  src/materials/MirrorMaterial.cpp \
                  src/lights/PointLight.cpp \
                  src/lights/DirectionalLight.cpp \
                  src/lights/DirectionalColorLight.cpp \
                  src/lights/SpotLight.cpp \
                  src/lights/ColorLight.cpp \
                  src/transforms/TranslationTransform.cpp \
                  src/transforms/ScaleTransform.cpp \
                  src/transforms/RotationTransform.cpp \
                  src/primitives/Torus.cpp \
                  src/materials/TransparencyMaterial.cpp \
                  src/rendering/LowRendering.cpp \

OBJ = $(SRC:%.cpp=%.o)

all: $(NAME) plugins

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

clean:
	rm -rf $(OBJ)
	rm -f output.ppm

fclean: clean
	rm -f $(NAME)
	rm -rf plugins

re: fclean all

plugins:
	@mkdir -p plugins
	@set -e; for file in $(SRC_PLUGINS); do \
		name=$$(basename $$file .cpp); \
		echo "Compiling plugins/$$name.so..."; \
		$(CXX) -shared -fPIC $(CXXFLAGS) $(CPPFLAGS) $$file -o plugins/$$name.so $(LDFLAGS); \
	done

docs:
	@command -v doxygen >/dev/null 2>&1
	@doxygen Doxyfile
	@echo "Docs generated at docs/doxygen/html"

.PHONY: all clean fclean re plugins docs
