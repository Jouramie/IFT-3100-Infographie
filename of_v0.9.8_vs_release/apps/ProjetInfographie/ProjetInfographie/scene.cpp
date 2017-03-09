#include "scene.h"

using namespace std;

Scene::Scene() : root(0)
{
	
}

void Scene::addElement(std::shared_ptr<primitive>& e)
{

}

void Scene::addElement(int index, std::shared_ptr<primitive>& e)
{
	root.addElement(index, e);
}

void Scene::selectElement(int index)
{

}

void Scene::rebuildIndex(int from)
{
}


/************************************************************************/
/*							SceneGroup		                            */
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

//Ajouter l'élément directement après l'élément à l'index en paramètre
void Scene::SceneGroup::addElement(int index, std::shared_ptr<primitive>& e)
{
	SceneElement::addElement(index, e);

	if (this->index == index) {
		addElement(e);
	}
	else {
		//TODO à tester
		int ubound = childrens.size();
		int lbound = 0;
		int i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				lbound = ubound;
				//Inserer l'élément après i
				size++;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound == lbound) {
					//Ajoute l'élément dans le groupe soujacent
					childrens[i]->addElement(index, e);
					size++;
				}
			}
			else {
				ubound = i - 1;
				if (ubound == lbound) {
					//Ajoute l'élément dans le groupe soujacent
					childrens[i - 1]->addElement(index, e);
					size++;
				}
			}
		}
	}
}

//Retire l'élément à l'index en paramètre
void Scene::SceneGroup::removeElement(int index)
{
	SceneElement::removeElement(index);

	if (this->index == index) {
		removeElement();
	}
	else {
		//TODO à tester
		int ubound = childrens.size();
		int lbound = 0;
		int i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				lbound = ubound;
				//Retirer l'élément i
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound == lbound) {
					//Retier l'élément dans le groupe soujacent
					int size = childrens[i]->getSize();
					childrens[i]->removeElement(index);
					this->size -= size;
				}
			}
			else {
				ubound = i - 1;
				if (ubound == lbound) {
					//Retirer l'élément dans le groupe soujacent
					int size = childrens[i - 1]->getSize();
					childrens[i - 1]->removeElement(index);
					this->size -= size;
				}
			}
		}
	}
}

void Scene::SceneGroup::addElement(std::shared_ptr<primitive>& e)
{

}

void Scene::SceneGroup::removeElement()
{

}


/************************************************************************/
/*							SceneNode		                            */
/************************************************************************/

Scene::SceneNode::SceneNode(int index, shared_ptr<primitive>& content) : SceneElement(index, 1), content{ content } { }

void Scene::SceneNode::addElement(int index, std::shared_ptr<primitive>& e)
{
	SceneElement::addElement(index, e);
	if (index != this->index && index != 0) {
		throw invalid_argument("index need to be equals to the index of the node, or 0");
	}
	else {
		this->content = e;
	}
}

void Scene::SceneNode::removeElement(int index)
{
	SceneElement::removeElement(index);
	if (index != this->index && index != 0) {
		throw invalid_argument("index need to be equals to the index of the node, or 0");
	}
	else {
		this->content = NULL;
	}
}

void Scene::SceneNode::addElement(std::shared_ptr<primitive>& e)
{
}

void Scene::SceneNode::removeElement()
{
}


/************************************************************************/
/*							SceneElement	                            */
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
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	} else {
		this->index = index;
	}
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

void Scene::SceneElement::addElement(int index, std::shared_ptr<primitive>& e)
{
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	}
}

void Scene::SceneElement::removeElement(int index)
{
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	}
}
