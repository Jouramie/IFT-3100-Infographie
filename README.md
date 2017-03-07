# README #

##Pour configurer ofxGui##

1. Allez dans Projet > Propriété > c/c++ > général > Autres répertoires Include
2. Cliquez et modifiez les autres répertoires Include (flèche a droite)
3. Ajoutez un répertoire en cliquant sur le petit dossier avec un soleil
4. Explorez pour trouver les fichiers source de l'addons ofxGui. Pour se faire, retournez à la racine de OpenFrameworks puis choisissez addons > ofxGui > src
5. Confirmez le tout et ça devrait fonctionner!






***********************
# NE PAS INSTALLER QT #
***********************

##Pour installer Qt##

1. Se rendre sur [le release 5.7 de Qt](https://www.qt.io/qt5-7/)
2. Répondre aux questions demandés.
3. Exécuter l'installateur (Spamer le next)

##Pour ajouter le Add-in Qt VS tools sur Visual Studio##

1. Aller dans Outils > Extensions & mises à jour > En ligne > Rechercher Qt VS Tools
2. Installer le plug-in
3. Redémarrer VS
4. Dans Visual Studio, sélectionner Qt VS Tools > Qt Options. 
5. Cliquer sur Add, entrer un nom au choix (Ex: Qt 5.7) et entrer le path: ...\Qt\5.7\msvc2015_64

##Pour configurer Qt creator avec openframeworks##

1. Suivre les étapes sur [ce lien](http://openframeworks.cc/setup/qtcreator/) 
2. Il demande d'installer msys2. Suivez [ces étapes.](http://openframeworks.cc/setup/msys2/)
3. Ensuite, après la mises à jour des modules de msys2, il faut copier les templates de OF (scripts/qtcreator/templates) vers c:\Qt\qtcreator-3.5.1\share\qtcreator\templates