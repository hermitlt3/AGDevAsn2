#ifndef WINDMILL_H
#define WINDMILL_H

class GenericEntity;
class Windmill
{
public:
	Windmill();
	~Windmill();

	void Update(double dt);
	//void Render();

private:
	GenericEntity* parts[3];
};

#endif 