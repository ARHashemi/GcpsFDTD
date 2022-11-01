#include "projecttree.h"
#include <QMessageBox>

ProjectTree::ProjectTree(QDomDocument *project_File)//, QTreeWidget *project_Tree
{    
    projectFile = project_File;
//    projectTree = project_Tree;

//    QDomNodeList childList = project_File->childNodes();
//    for(int i=0; i<childList.size(); ++i){
//        out << childList.at(i).nodeName() << "\t" << childList.at(i).nodeValue() << "\n";
//    }
    rootNode = project_File->namedItem("Project");
//    childList = rootNode.childNodes();
//    for(int i=0; i<childList.size(); ++i){
//        out << childList.at(i).nodeName() << "\t" << childList.at(i).nodeValue() << "\n";
//    }
//    QDomElement childElement = rootNode.firstChildElement();
//    while (!childElement.isNull()) {
//        out << childElement.tagName() << "\t" << childElement.text() << "\t" << childElement.localName() << "\t" << childElement.nodeName() << "\t" << childElement.nodeValue() << "\n";
//        childElement = childElement.nextSiblingElement();//
//    }


    if(!rootNode.isNull() && rootNode.hasAttributes()){
        const QDomNamedNodeMap attributeMap = rootNode.attributes();
        projectTitle = attributeMap.namedItem("title").nodeValue();
        qStdOut() << projectTitle << "\n";
    }else{       
        errFileIncorrect();
    }

    projectTreeModel = new QStandardItemModel(0,2);
    rootItem = projectTreeModel->invisibleRootItem();
    firstColumnFont = rootItem->font();
    firstColumnFont.setBold(true);
    QList<QStandardItem *> titleRow = {new QStandardItem("Title"), new QStandardItem(projectTitle)};
    titleRow.at(0)->setFont(firstColumnFont);
    rootItem->appendRow(titleRow);
    rootItem->setEditable(false);

    extractStructure();
    extractMaterials();

}

void ProjectTree::extractStructure()
{
    structureNode = rootNode.namedItem("Structure");
    if(!structureNode.isNull()){
        structureItem = new QStandardItem();
        structureItem->setText("Structure");
        structureItem->setFont(firstColumnFont);
        structureItem->setEditable(false);
        if(structureNode.hasAttributes()){
            const QDomNamedNodeMap attributeMap = structureNode.attributes();
            unit = attributeMap.namedItem("unit").nodeValue();
            freeSpace = attributeMap.namedItem("freeSpace").nodeValue().toInt();

        }else{
            errFileMissedItem("Unit is unspecified!");
        }
        QDomNode dimensionsNode = structureNode.namedItem("Dimensions");
        if(!dimensionsNode.isNull() && dimensionsNode.hasAttributes()){            
            const QDomNamedNodeMap attributeMap = dimensionsNode.attributes();
            Lx = attributeMap.namedItem("Lx").nodeValue().toDouble();
            Ly = attributeMap.namedItem("Ly").nodeValue().toDouble();
            Lz = attributeMap.namedItem("Lz").nodeValue().toDouble();
            dx = attributeMap.namedItem("dx").nodeValue().toDouble();
            dy = attributeMap.namedItem("dy").nodeValue().toDouble();
            dz = attributeMap.namedItem("dz").nodeValue().toDouble();
            QStandardItem *dimensionsItem = new QStandardItem();
            dimensionsItem->setText("Dimensions");
            dimensionsItem->setFont(firstColumnFont);
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("Lx"), new QStandardItem(QString::number(Lx))}));
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("Ly"), new QStandardItem(QString::number(Ly))}));
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("Lz"), new QStandardItem(QString::number(Lz))}));
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("dx"), new QStandardItem(QString::number(dx))}));
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("dy"), new QStandardItem(QString::number(dy))}));
            dimensionsItem->appendRow(QList<QStandardItem *>({new QStandardItem("dz"), new QStandardItem(QString::number(dz))}));
            structureItem->appendRow(dimensionsItem);
//            qStdOut() << "Lx=" << Lx << "\n";
        }else{
            errFileMissedItem("No dimensional data!");
        }
        QDomNodeList shapeNodes = structureNode.toElement().elementsByTagName("Shape");
        if (shapeNodes.size()>0){
            NoShapes = shapeNodes.size();
            QStandardItem *shapesItem = new QStandardItem();
            shapesItem->setText("Shapes");
            shapesItem->setFont(firstColumnFont);
            shapes.resize(shapeNodes.size());
            for(int i=0; i<shapeNodes.size(); ++i){
                QStandardItem *shapeItem = new QStandardItem();
                shapeItem->setText("Shape");
                if(shapeNodes.at(i).hasAttributes()){
                    const QDomNamedNodeMap attributeMap = shapeNodes.at(i).attributes();
                    shapes[i].type = attributeMap.namedItem("type").nodeValue().toStdString();
                    shapes[i].index = attributeMap.namedItem("shapeIndex").nodeValue().toInt();
                    shapes[i].material_index = attributeMap.namedItem("materialIndex").nodeValue().toInt();
                    shapes[i].theta = attributeMap.namedItem("theta").nodeValue().toDouble();
                    shapes[i].phi = attributeMap.namedItem("phi").nodeValue().toDouble();
                    shapes[i].psi = attributeMap.namedItem("psi").nodeValue().toDouble();
                    shapes[i].x0 = attributeMap.namedItem("x0").nodeValue().toDouble();
                    shapes[i].y0 = attributeMap.namedItem("y0").nodeValue().toDouble();
                    shapes[i].z0 = attributeMap.namedItem("z0").nodeValue().toDouble();
                    shapes[i].a = attributeMap.namedItem("a").nodeValue().toDouble();
                    shapes[i].b = attributeMap.namedItem("b").nodeValue().toDouble();
                    shapes[i].c = attributeMap.namedItem("c").nodeValue().toDouble();
                    shapeItem->setText(QString::fromStdString(shapes[i].type));
                    shapeItem->setFont(firstColumnFont);
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("index"), new QStandardItem(QString::number(shapes[i].index))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("materialIndex"), new QStandardItem(QString::number(shapes[i].material_index))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("theta"), new QStandardItem(QString::number(shapes[i].theta))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("phi"), new QStandardItem(QString::number(shapes[i].phi))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("psi"), new QStandardItem(QString::number(shapes[i].psi))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("x0"), new QStandardItem(QString::number(shapes[i].x0))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("y0"), new QStandardItem(QString::number(shapes[i].y0))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("z0"), new QStandardItem(QString::number(shapes[i].z0))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("a"), new QStandardItem(QString::number(shapes[i].a))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("b"), new QStandardItem(QString::number(shapes[i].b))}));
                    shapeItem->appendRow(QList<QStandardItem *>({new QStandardItem("c"), new QStandardItem(QString::number(shapes[i].c))}));
                    shapesItem->appendRow(shapeItem);
                }else{
                    errFileMissedItem("Missed shape data!");
                }
            }
            structureItem->appendRow(shapesItem);
        }
    }else{
        errFileIncorrect();
    }
    rootItem->appendRow(structureItem);
}

void ProjectTree::extractMaterials()
{
    materialsNode = rootNode.namedItem("Materials");
    if(!materialsNode.isNull()){
        materialsItem = new QStandardItem();
        materialsItem->setText("Materials");
        materialsItem->setFont(firstColumnFont);
        materialsItem->setEditable(false);
        QDomNodeList materialNodes = materialsNode.toElement().elementsByTagName("Material");
        if (materialNodes.size()>0){
            NoMaterials = materialNodes.size();
            materials.resize(materialNodes.size());
            for(int i=0; i<materialNodes.size(); ++i){
                QStandardItem *materialItem = new QStandardItem();
                materialItem->setText("Material");
                if(materialNodes.at(i).hasAttributes()){
                    const QDomNamedNodeMap attributeMap = materialNodes.at(i).attributes();
                    materials[i].index = attributeMap.namedItem("materialIndex").nodeValue().toInt();
                    materials[i].isMetal = attributeMap.namedItem("isMetal").nodeValue().toInt();
                    materials[i].isFluorescent = attributeMap.namedItem("isFluorescent").nodeValue().toInt();
                    materials[i].epsR = attributeMap.namedItem("epsR").nodeValue().toDouble();
                    materials[i].muR = attributeMap.namedItem("muR").nodeValue().toDouble();
                    materials[i].sig = attributeMap.namedItem("sig").nodeValue().toDouble();
                    materials[i].sigM = attributeMap.namedItem("sigM").nodeValue().toDouble();
                    materialItem->setText(attributeMap.namedItem("name").nodeValue());
                    materialItem->setFont(firstColumnFont);
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("index"), new QStandardItem(QString::number(materials[i].index))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("epsR"), new QStandardItem(QString::number(materials[i].epsR))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("muR"), new QStandardItem(QString::number(materials[i].muR))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("sig"), new QStandardItem(QString::number(materials[i].sig))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("sigM"), new QStandardItem(QString::number(materials[i].sigM))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("isMetal"), new QStandardItem(QString::number(materials[i].isMetal))}));
                    materialItem->appendRow(QList<QStandardItem *>({new QStandardItem("isFluorescent"), new QStandardItem(QString::number(materials[i].isFluorescent))}));
                    materialsItem->appendRow(materialItem);
                }else{
                    errFileMissedItem("Missed material data!");
                }
            }
        }
    }else{
        errFileIncorrect();
    }
    rootItem->appendRow(materialsItem);
}

void ProjectTree::errFileIncorrect()
{
    qStdOut() << "This is not a correct FDTD project file.\n";
    QMessageBox msgBox;
    msgBox.setText("This is not a correct FDTD project file!");
    msgBox.exec();
}

void ProjectTree::errFileMissedItem(QString message)
{
    qStdOut() << "Some required data is missed in file:\n" << message << "\n";
    QMessageBox msgBox;
    msgBox.setText(QString("Some required data is missed in file:\n").append(message));
    msgBox.exec();
}

QTextStream &ProjectTree::qStdOut()
{
    static QTextStream ts( stdout );
    return ts;
}
