#ifndef PROJECTTREE_H
#define PROJECTTREE_H

#include <QTreeWidget>
#include <QStandardItemModel>
#include <QDomDocument>
#include <QTextStream>

#include "Structure.h"

class ProjectTree : QObject
{
    Q_OBJECT
public:
//    ProjectTree(QDomDocument *project_File, QTreeWidget *project_Tree);
//    ProjectTree(QDomDocument *project_File, QStandardItemModel *project_Tree_model);
    ProjectTree(QDomDocument *project_File);

    void extractStructure();
    void extractMaterials();
    void errFileIncorrect();
    void errFileMissedItem(QString message);
    QDomDocument *projectFile;
//    QTreeWidget *projectTree;
    QStandardItemModel *projectTreeModel;
    QDomNode rootNode, structureNode, materialsNode;
    QStandardItem *rootItem, *structureItem, *materialsItem;
    QString projectTitle;
    QString unit;
    bool freeSpace=false;
    double Lx, Ly, Lz, dx, dy, dz;
    QList <shape> shapes;
    int NoShapes;
    QList <material> materials;
    int NoMaterials;

private:
    QTextStream& qStdOut();
    QFont firstColumnFont;
};

#endif // PROJECTTREE_H
