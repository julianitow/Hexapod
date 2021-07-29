# Hexapod

[![forthebadge](http://forthebadge.com/images/badges/built-with-love.svg)](http://forthebadge.com)  [![forthebadge](http://forthebadge.com/images/badges/powered-by-electricity.svg)](http://forthebadge.com)

Hexapod est un projet complètement OpenSource, toujours en développement. Ce robot à 6 pattes controllé par une appliucation mobile en flutter est un parfait projet pour débuter en robotique. Il est composé, d'un réveil avec un buzzer, de capteurs ultrasons pour le déplacement, d'un écran LCD pour afficher les informations. Pour commencer il vous suffit d'un raspberry pi, de legos, et de motivation !

## Pour commencer

Ce repo contient ce que l'on peut appeler, l'âme du pod. Toute sa matière grise se trouve ici. Un serveur WebSocket multithreadé qui éxecute les scripts python qui controllent ses composants 

### Pré-requis

 - Une bonne dose de caféine
 - Une installation de QT OpenSource sur le raspberry pi:
 [QT Cross Compile](https://doc.qt.io/archives/qt-4.8/qt-embedded-crosscompiling.html)
 - Une machine virtuelle Debian si vous êtes sur Windows

## Installation

Une fois QT installé sur les plateformes \
- ```make```
- ```scp WebSocketServer pi@hexapod.local:/home/pi/Server_Run```

## Démarrage

```sudo systemctl start hexapod```

## Fabriqué avec

Développé sous QT avec la librairie
[QTWebSocket](https://github.com/qt/qtwebsockets)

## Contributing

Si vous souhaitez contribuer, lisez le fichier [CONTRIBUTING.md](https://example.org) pour savoir comment le faire.

## Versions
Listez les versions ici 
_exemple :_
**Dernière version stable :** 1.0

## Auteurs
Listez le(s) auteur(s) du projet ici !
* **Sandrine Patin** _alias_ [@Sandrine Patin](https://github.com/SandrinePatin)
* **Julien Guillan** _alias_ [@Julien Guillan](https://github.com/julien-guillan)


## License

Ce projet est sous licence ``MIT`` 


