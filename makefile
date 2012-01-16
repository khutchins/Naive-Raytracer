all:
	g++ src/EasyBMP/*.cpp src/*.cpp -o Naive-Raytracer

clean:
	rm -rf *.o Naive-Raytracer
