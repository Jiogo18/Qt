#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "Point3D.h"
#include "Object3D.h"
#include <QVector>
#include <QPainter>
#include <QPointer>
#include <QThread>
#include "map3D.h"

#include "DebugTime.h"

namespace RAYTRACING {
    const doubli viewDistance = 100;
    const doubli angleH = 120;//de 156° à 172° max pour un Humain (1 oeil)
    const doubli angleV = 100;//180° = inf, 0° = 0
    const doubli xMax = tan(RAYTRACING::angleH/2);
    const doubli yMax = tan(RAYTRACING::angleV/2);
    const int pppH = 1;
    const int pppV = 1;
    const int ppp = pppV * pppH;
    const int WorkerThread = 13;
    const int RefreshColumn = 100;
    const int gamma = 2;// TODO: réduire modifier ça lorsqu'on aura la lumière du soleil
}
using namespace RAYTRACING;



class Plan
{
public:
    Plan();
    Plan(const Point3D &pA, const Point3D &pB, const Point3D &pC);
    Plan(const Rect3D &rect);
    Plan(const Plan &plan);
    void calcEquation();//ax + by + cz + d = 0
    //mais avec a = 1 ! (colinéaire donc pas besoin de chercher plus...)
    bool paralleleDroite(const Point3D &pA, const Point3D &pB) const;
    Point3D intersection(const Point3D &pA, const Point3D &pB) const;
    QPointF projeteOrtho(const Point3D &pA) const;
    bool isValue() const { return a!=0.0L || b!=0.0L || c!=0.0L || d!=0.0L; }
    Plan *operator =(const Plan &plan);
private:
    Point3D pA;
    Point3D pB;
    Point3D pC;
    doubli a=0, b=0, c=0, d=0;
};



template<typename T>
class PixScreen
{
public://il faut au moins un PixScreen<T> dans ce fichier pour en avoir avec le même type dans d'autres fichiers
    PixScreen() { screen = QList<QList<T>>(0); };
    PixScreen(const QSize &size);
    PixScreen(const PixScreen &ps) { screen = ps.screen; };
    PixScreen *operator=(const PixScreen &ps) { screen = ps.screen; return this; };
    void set(const int &x, const int &y, T pos) { screen[x][y] = pos; };
    T at(const int &x, const int &y) const { return screen.at(x).at(y); };
    int width() const { return screen.length(); };
    int height() const { return screen.length() ? screen.at(0).length() : 0; };
    QSize size() const { return QSize(width(), height()); }
    int rowNumber() const { return width() * height(); };
    QList<T> getColumn(int x) const { return screen.at(x); };
    void setColumn(int x, QList<T> column) { screen[x] = column; };
private:
    QList<QList<T>> screen;
};


class RayTracingRessources
{
public:
    RayTracingRessources(const World *world, const Pos3D &clientPos, DebugTime *dt);
    ~RayTracingRessources();
    const World *world = nullptr;
    DebugTime *dt = nullptr;
    Pos3D clientPos;
    QMap<const Face*, const Plan*> *facesPlan = nullptr;
    QMap<QString, const QImage*> *facesImg = nullptr;
    void worldChanged();
};



class Ray
{
public:
    Ray(Pos3D pos, RayTracingRessources *rtRess);
    ColorLight getColor() const;
    void process(const World *world);
private:
    Pos3D origin;
    Pos3D pos;
    Point3D posNextPoint;
    QList<ColorLight> colors;
    const Face *lastFace = nullptr;
    bool lastMoved = false;
    int opacity = 0;
    //vector objTraverse;
    doubli distParcouru;
    void moveTo(const Pos3D &pos);
    Pos3D getPos() const { return pos; }
    const Face *getFirstIntersection(const World *world, Point3D *pInter) const;
    //black list (et white list pour transparence) du dernier objet traversé
    //par défaut: sortie du client (pour pas le retapper)
    //si rebond: dernier objet en mode "sortie"
    //si entrée (transparent): dernier objet en mode "entrée" puis "sortie" pour calculer l'épaisseur
    //si un objet dans l'objet entré est touché alors on sort du premier objet au point P et on rentre ds le 2
    //puis on resort du 2 on rerentre ds le 1 et resort du 1 (de l'autre coté de 2)
    bool enter = false;
    RayTracingRessources *rtRess = nullptr;
    const Plan *getPlan(const Face *face) const;
    const QImage *getTexture(const QString &file) const;
};



class RayTracingWorker : public QThread
{
    Q_OBJECT
public:
    RayTracingWorker(int workerId, RayTracingRessources *rtRess, QObject *parent = nullptr);
    RayTracingWorker *setPrimaryWork(const QList<doubli> &yPosList, const QSize &sceneSize);
    RayTracingWorker *setWork(int xScene) { this->xScene = xScene; return this; };
    int getWorkerId() const { return workerId; };
    static QList<doubli> calcyPosList(const int &sceneHeight);

signals:
    void resultReady(RayTracingWorker *worker, int y, const QList<ColorLight> &c, int totalLight);
private:
    static QList<Pos3D> calcRaysPosColumn(const int &x, const QSize &sceneSize, const Pos3D &clientPos, const QList<doubli> &yPosList);
    void run() override;// process ONE column at a time
    int workerId;
    QList<doubli> yPosList;
    QSize sceneSize;

    int xScene;// just ONE column
    RayTracingRessources *rtRess;
};


class RayTracing : public QThread
{
    Q_OBJECT
public:
    RayTracing(const map3D *map);
    ~RayTracing();

    RayTracing *setSize(const QSize &size) { if(!isRunning()) calcSize = size; return this; }
    const QImage &getImage() const { return image; }
    bool isRunning() { return QThread::isRunning() || workersInProcess; }

private slots:
    void worldChanged(const WorldChange &change);
    void onRayWorkerReady(RayTracingWorker *worker, int x, const QList<ColorLight> &c, int totalLight);

signals:
    void resultReady(const QImage &image);

private:
    void run() override;
    void assignNextRayWork(RayTracingWorker *worker);
    double calcTotalLight() const;
    void paint();
    void onAllWorkersFinished();

    RayTracingRessources *rtRess = nullptr;
    DebugTime dt;
    qint64 startRun;

    const map3D *map;
    QSize calcSize;

    QList<doubli> yPosList; // calculs des yPos en fonction de y
    PixScreen<ColorLight> colors;
    QList<double> lightPerColumn; // calculs des light de chaque colonne (en fonction de x)
    int processToStart = 0;
    int processDone = 0;
    int processStarted = 0;
    QList<RayTracingWorker*> rayWorkers;
    bool workersInProcess = false;
    QImage image;
};


/*
tests: equation du rayon:
z = sin(aH) * x => mais ça réduit si on augmente aV donc NON

y augmente? quand distance_origine augmente et que aV > 0
y = sin(aV) * distance_origine = sin(aV) * sqrt(x²+z²)
t temp en s
y = sin(aV)*t
x = cos(aH)*t*cos(aV)
z = sin(aH)*t*cos(aV)
<=> t = y/sin(aV) = x/cos(aH)cos(aV) = z/sin(aH)cos(aV)
<=> z/sin(aH) = x/cos(aH)
<=> z*cos(aH) = x*sin(aH)
<=> z = x*sin(aH)/cos(aH)
mais si aH = 90... ça marche plus
=> en 2D ça fait la même: y=a ou x=a (on peut pas mettre de x ou de y dans ces cas

un point en 1D? ax+b = 0
une droite en 2D? ax+by+c = 0
une plan en 3D? ax+by+cz+d = 0
il faut trouver l'équation du plan ou du volume du cube (?)
la droite il faut le point d'origine (client) et calculer un autre point => pour avoir le vecteur


equation d'un plan:
https://www.maxicours.com/se/cours/equation-cartesienne-d-un-plan/
https://www.youtube.com/watch?v=s4xqI6IPQBY
intersection d'un plan et d'une droite:
https://youtu.be/BYBMauyizhE

Pour commencer: #Orthogonal
verifier que AB et le plan P sont sécants:
=> si vecteur normal de P est orthogonal à AB alors on s'arrete
m(a, b, c) est un vecteur normal de P avec l'équation de P: ax+by+cz+d = 0
xm*xAB + ym*yAB + zm*zAB = 0 ? alors on s'arrete

#Intersection
Droite:
x = xA + xAB*t
y = yA + yAB*t
z = zA + zAB*t
Plan:
ax+by+cz+d = 0
<=> a*(xAB*t + xA) + b*(yAB*t + yA) + c*(zAB*t + zA) + d = 0
<=> (a*xAB + b*yAB + c*zAB)*t = -a*xA -b*yA -c*zA - d
<=> t = -(a*xA + b*yA + c*zA + d)/(a*xAB + b*yAB + c*zAB)


plus qu'a trouver a, b et c...
On connait 4 points du plan, on s'en sert que de 2
il faut donc trouver un vecteur normal n => perpendiculaire à 2 droites du plan => on prend AB et AC

n perpendiculaire AB et AC (et AB pas colinéaire à AC => pas le meme angle)
<=> n.AB = a*xAB + b*yAB + c*zAB = 0
<=> n.AC = a*xAC + b*yAC + c*zAC = 0


#EquationPlan
on a:	n.AB = xAB*a+yAB*b+zAB*c = 0
        n.AC = xAC*a+yAC*b+zAC*c = 0
1) on veut yAB*b - n*yAC*b = 0 (n Réel)
<=> nb = yAB/yAC
on a:
n.AB - n*n.AC = xAB*a + zAB*c - n(xAC*a + zAC*c) = 0
<=> c(zAB-zAC*n) = -(xAB-xAC*n)*a = (xAC*n-xAB)*a
<=> c = (xAC*n-xAB)*a/(zAB-zAC*n)

2)pareil avec zAB*c - n*zAC*c = 0 (n Réel)
<=> nc = zAB/zAC
<=> b = (xAC*n-xAB)*a/(yAB-yAC*n)

test avec -3a+b+2c = 0 et -7a-2b+4c = 0 (maxicours)
nb = yAB/yAC = 1/-2 = -1/2
c = (-7*-1/2 - -3)*a/(2 - 4*-1/2)
  = (7/2 + 3)*a/(2 + 2)
  = (13)/2*a/4
  = 13*a/8 OK
nc = zAB/zAC = 2/4 = 1/2
b = (-7*1/2 - -3)*a/(1 - -2*1/2)
  = (-7+6)/2 *a/(1+1)
  = -1*a/4 = -a/4 OK

après on détermine a comme on veut (1 par exemple)

on cherche d
on sait que A appartient au plan donc l'équation = 0
=> a*xA + b*yA + c*zA = -d

*/

#endif // RAYTRACING_H
