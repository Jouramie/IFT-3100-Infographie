#include "scene.h"

using namespace std;

Scene::Scene() : root(0)
{
	
}

void Scene::selectElement(int index)
{

}

void Scene::rebuildIndex(int from)
{
}


/************************************************************************/
/*							Scene Group		                            */
/************************************************************************/


Scene::SceneGroup::SceneGroup(int index) : SceneElement(index, 0) {}

void Scene::SceneGroup::setVisible(bool visible)
{
	SceneElement::setVisible(visible);
	for (auto i : childrens) {
		i->setVisible(visible);
	}
}

void Scene::SceneGroup::setLocked(bool locked)
{
	SceneElement::setLocked(locked);
	for (auto i : childrens) {
		i->setLocked(locked);
	}
}

void Scene::SceneGroup::setIndex(int index)
{
	if (index >= 0) {
		for (auto i : childrens) {
			i->setIndex(i->getIndex() + (index - this->index));
		}
		SceneElement::setIndex(index);
	}
	else {
		throw invalid_argument("index need to be greater than 0");
	}
}

void Scene::SceneGroup::addElement(int index, std::shared_ptr<primitive>& e)
{
	//TODO à tester

	bool found = false;
	int ubound = childrens.size();
	int lbound = 0;
	int i;

	while (lbound <= ubound && !found) {
		i = lbound + (ubound - lbound) / 2;
		if (childrens[i]->getIndex() == index) {
			found = true;
		}
		else if (childrens[i]->getIndex() < index) {
			lbound = i + 1;
			if (ubound == lbound) {
				childrens[i]->addElement(index, e);
			}
		}
		else {
			ubound = i - 1;
			if (ubound == lbound) {
				childrens[i-1]->addElement(index, e);
			}
		}
	}

	if (found) {
		//Ajouter l'élément juste après i
	}
}

std::shared_ptr<SceneElement>& Scene::SceneGroup::getElement(int index)
{
	// TODO: insert return statement here
}


/************************************************************************/
/*							Scene Node		                            */
/************************************************************************/

Scene::SceneNode::SceneNode(int index, shared_ptr<primitive>& content) : SceneElement(index, 1), content{ content } { }

void Scene::SceneNode::addElement(int index, std::shared_ptr<primitive>& e)
{
	if (index == this->index || index == 0) {
		this->content = e;
	}
	else {
		throw invalid_argument("index need to be equals to the index of the node, or 0");
	}
}

std::shared_ptr<SceneElement>& Scene::SceneNode::getElement(int index)
{
	// TODO: insert return statement here
}


/************************************************************************/
/*							Scene Element                               */
/************************************************************************/

Scene::SceneElement::SceneElement(int index) : SceneElement{ index, 0 } {}

Scene::SceneElement::SceneElement(int index, int size) : visible{ true }, locked{ false }, size{ size } {}

int Scene::SceneElement::getSize()
{
	return size;
}

int Scene::SceneElement::getIndex()
{
	return index;
}

void Scene::SceneElement::setIndex(int index)
{
	this->index = index;
}

void Scene::SceneElement::setVisible(bool visible)
{
	this->visible = visible;
}

bool Scene::SceneElement::isVisible()
{
	return visible;
}

void Scene::SceneElement::setLocked(bool locked)
{
	this->locked = locked;
}

bool Scene::SceneElement::isLocked()
{
	return locked;
}
