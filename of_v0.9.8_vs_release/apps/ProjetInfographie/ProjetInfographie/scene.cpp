#include "scene.h"

using namespace std;

scene::scene() : root { 0 }, selectedIndex { 0 }
{
	
}

/*
Add the primitive at the selected index
*/
void scene::addElement(primitive_ptr& p)
{
	addElement(selectedIndex, p, true);
}

void scene::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	if (index == 0 && !insertFirstChild)
	{
		throw invalid_argument("root don't have parent...");
	}
	root.addElement(index, p, insertFirstChild);
}

void scene::removeElement(size_t index) 
{
	if (index == 0) {
		throw invalid_argument("can't remove root...");
	}
	root.removeElement(index);
}

void scene::selectElement(size_t index)
{
	selectedIndex = index;
}

/************************************************************************/
/*							SceneGroup		                            */
/************************************************************************/


scene::group::group(size_t index) : element(index, 0) {}

void scene::group::setIndex(size_t index)
{
	if (index >= 0) {
		for (auto& i : childrens) {
			i->setIndex(i->getIndex() + (index - this->index));
		}
		element::setIndex(index);
	}
	else {
		throw invalid_argument("index need to be greater than 0");
	}
}

/*
Ajouter l'élément directement après l'élément à l'index en paramètre
*/
void scene::group::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	element::addElement(index, p, insertFirstChild);

	//TODO à tester

	if (this->index == index) {
		if (insertFirstChild)
		{
			//Inserer comme premier element
			childrens.insert(childrens.begin(), element_ptr{ new node{ index + 1, p } });
			for (auto& it = childrens.begin() + 1; it < childrens.end(); ++it) {
				it->get()->setIndex(it->get()->getIndex() + 1);
			}
		}
		else {
			throw invalid_argument("element must to be add in the parent");
		}
	}
	else {
		size_t ubound = childrens.size();
		size_t lbound = 0;
		size_t i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				if (insertFirstChild) {
					childrens[i]->addElement(index, p, insertFirstChild);
				}
				else {
					childrens.insert(childrens.begin() + i, element_ptr{ new node{ index, p } });
				}
				i++;
				break;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound == lbound) {
					//Ajoute l'élément dans le groupe sous-jacent
					childrens[i]->addElement(index, p, insertFirstChild);
					i++;
				}
			}
			else {
				ubound = i - 1;
				if (ubound == lbound) {
					//Ajoute l'élément dans le groupe sous-jacent
					childrens[i - 1]->addElement(index, p, insertFirstChild);
				}
			}
		}
		size++;
		for (auto& it = childrens.begin() + i; it < childrens.end(); ++it) {
			it->get()->setIndex(it->get()->getIndex() + 1);
		}
	}
}

// return the size of the removed elements
size_t scene::group::removeElement(size_t index)
{
	size_t removedSize = element::removeElement(index);;

	//TODO à tester
	
	if (this->index == index) {
		//Retirer tous les enfants
		for (auto& i : childrens) {
			i->removeElement(i->getIndex());
		}
		childrens.clear();

	}
	else {
		size_t ubound = childrens.size();
		size_t lbound = 0;
		size_t i;

		while (lbound <= ubound) {
			i = lbound + (ubound - lbound) / 2;
			if (childrens[i]->getIndex() == index) {
				//Retirer l'élément i
				removedSize = childrens[i]->removeElement(index);
				childrens.erase(childrens.begin() + i);
				i++;
				break;
			}
			else if (childrens[i]->getIndex() < index) {
				lbound = i + 1;
				if (ubound == lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					removedSize = childrens[i]->removeElement(index);
					i++;
				}
			}
			else {
				ubound = i - 1;
				if (ubound == lbound) {
					//Retirer l'élément dans le groupe sous-jacent
					removedSize = childrens[i - 1]->removeElement(index);
				}
			}
		}

		this->size -= removedSize;
		for (auto& it = childrens.begin() + i; it < childrens.end(); ++it) {
			it->get()->setIndex(it->get()->getIndex() - removedSize);
		}
	}

	return removedSize;
}


/************************************************************************/
/*							SceneNode		                            */
/************************************************************************/

scene::node::node(size_t index, primitive_ptr& p) : element(index, 1), content{ p } { }

// insertFirstChild est ignoré
void scene::node::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	element::addElement(index, p, insertFirstChild);
	if (index != this->index) {
		throw invalid_argument("index need to be equals to the index of the node");
	}
	this->content = p;	
}


// return the size of the removed elements
size_t scene::node::removeElement(size_t index)
{
	size_t removedSize = element::removeElement(index);
	if (index != this->index) {
		throw invalid_argument("index need to be equals to the index of the node");
	}
	else {
		this->content = NULL;
	}

	return removedSize;
}


/************************************************************************/
/*							SceneElement	                            */
/************************************************************************/

scene::element::element(size_t index) : element{ index, 0 } {}

scene::element::element(size_t index, size_t size) : index{ index }, size{ size } {}

size_t scene::element::getSize()
{
	return size;
}

size_t scene::element::getIndex()
{
	return index;
}

void scene::element::setIndex(size_t index)
{
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	} else {
		this->index = index;
	}
}

void scene::element::addElement(size_t index, primitive_ptr& p, bool insertFirstChild)
{
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	}
}


// return the size of the removed elements
size_t scene::element::removeElement(size_t index)
{
	if (index < 0) {
		throw invalid_argument("index need to be greater than 0");
	}

	return size;
}
