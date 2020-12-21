#include "JSON.h"

JSON::JSON(debug *deb) {pdeb = deb; objJson=new JSON_Object;}

void JSON::loadFile(QString fileName)
{
    objJson = StrToJson(loadCode(fileName));
}

QString JSON::loadCode(QString fileName)
{
    if(fileName.isEmpty()) {
        pdeb->addLog("File/Load", "No filename", 2);
        return "";
    }
    QFile file(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if(!file.isOpen()) {
        pdeb->addLog("File/Load", "file can't be open : \""+fileName+"\" ("+file.errorString()+")", 1);
        return "";
    }

    QString code = file.readAll();
    file.close();
    pdeb->addLog("File/Load", "Loaded from \""+fileName+"\"", 3);
    return code;
}

void JSON::saveInFile(QString fileName)
{
    if(fileName.isEmpty()) {
        pdeb->addLog("File/Save", "No filename", 2);
    }
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        pdeb->addLog("File/Save", "file can't be open : \""+fileName+"\" ("+file.errorString()+")", 1);

    QTextStream out(&file);
    out << JsonToStr(objJson);
    file.close();

    pdeb->addLog("File/Save", "Saved in \""+fileName+"\"", 3);
}

void JSON::execCmd(QString cmd)
{
    if(cmd.startsWith("help")) {
        pdeb->addLog("Commands", QString("help : affiche cette liste\n")+
                                        "[link] : pour accéder à une valeure/objet, format : name1.name2\n"+
                                        "get [link] : retourne la valeure de l'objet\n"+
                                        "set [link] [value] : modifie la valeure de l'objet ([value] est un str par défaut)", 3);
    }
    else if (cmd.startsWith("get ") || cmd=="get") {//si ça commence par get xxx ou si c'est juste get
        QString value="";
        QString path=cmd;
        path.remove("get");//retire le get
        if(!path.isEmpty())//si il y a un (seul) espace, on le retire (si cmd=="get" ce sera pas le cas
            path.remove(" ");
        path = path.split(" ").at(0);//garde que l'argument actuel
        QList<QString> listeName=path.split(".");
        if(listeName.size() == 1 && listeName.at(0) == "") {
            listeName.clear();//ça arrive qu'il créé une case mais vide
        }
        JSON_Object *tempObj=objJson;
        bool done=false;
        for (int i=0; i<listeName.size(); i++ && !done) {
            if(tempObj->getChild(listeName.at(i)) != nullptr) {
                tempObj = tempObj->getChild(listeName.at(i));
            }
            else if (tempObj->containsValue(listeName.at(i))) {//la valeur
                pdeb->addLog("Commands", cmd+" = "+tempObj->getValue(listeName.at(i))+".", 3);
                done=true;
            }
            else {
                pdeb->addLog("Commands", cmd+" donnée inconnue : "+listeName.at(i)+" ("+QString::number(i+1)+"/"+QString::number(listeName.size())+").", 1);
                done=true;
            }
        }
        if(!done) {//si il y a rien de donné encore
            if(tempObj!=nullptr) {
                if(tempObj->containsValue("value")) {//si il contient
                    pdeb->addLog("Commands", cmd+" = "+tempObj->getValue("value")+" (type:"+tempObj->getTypeString()+").", 3);
                }
                else {
                    //pour la description de l'objet, mettre le type et ses child/valeure
                    pdeb->addLog("Commands", cmd+" = type:"+tempObj->getTypeString()+", entrees:"+tempObj->getEntress()+".", 3);
                }
                if(value=="")
                    value = "JSON_Object: "+QString::number(tempObj->getType());
            }
        }
    }
    else {
        pdeb->addLog("Commands", "Unknow : "+cmd+".", 1);
    }
}

JSON_Object *JSON::StrToJson(QString str)
{//on peut pas enlever les espace à cause des string inclus (ferais le bazard)
    JSON_Object *obj = new JSON_Object;
    int iStart=str.size();
    QString charBlock="";
    for(int i=0; i<block.size(); i+=2) {
        if(str.indexOf(block.at(i)) < iStart && str.indexOf(block.at(i))!=-1) {
            charBlock = block.at(i);
            iStart=str.indexOf(charBlock);
        }
    }
    QString b = getBlock(str, iStart);//block
    int i=0;
    int nbArgSansKey=0;
    obj->setType(typeOfValue(charBlock));
    switch (obj->getType()) {
        case JSON_Object::Object:
        case JSON_Object::Array:
        case JSON_Object::Argument: {
            while(i < str.size()) {
                i=nextChar(b, i);
                if(i==-1) break;
                QString currentLine = b.mid(i, endOfElement(b, i)-i+1);
                i += currentLine.size()+1;
                QString arg1="";
                QString arg2="";
                //if(block.contains(currentLine.at(0))) {//c'est un block
                    arg1 = getBlock(currentLine, 0);
                    currentLine.remove(currentLine.indexOf(arg1), arg1.size());//removeFirst
                //}
                currentLine = removeUseless(currentLine);
                int iL=0;

                //quel est le nouveau charactère?
                if(!currentLine.isEmpty() && currentLine.at(iL) == ":") {//c'est une définition
                    iL=nextChar(currentLine, iL);
                    currentLine.remove(0, iL);//on enleve, on sais ce que c'est
                    iL=0;
                    arg2 = getBlock(currentLine, iL);
                    currentLine.remove(currentLine.indexOf(arg2), arg2.size());//removeFirst
                    if(!currentLine.isEmpty()) {//euh? pk il lui reste encore des trucs.
                        pdeb->addLog("Load JSON", "Ah ! dans la ligne il reste :\n\""+currentLine+"\"\nmais pk? (arg1:"+arg1+", arg2:"+arg2+")", 0);
                        break;
                    }
                }
                else if(currentLine.isEmpty()) {//il y a rien d'autre
                    arg2 = arg1;
                    arg1 = QString::number(nbArgSansKey);//attention, remettre juste arg2 quand il faudra enregistrer
                    nbArgSansKey++;
                }
                else {//erreur, un charactère après le block
                    pdeb->addLog("Load JSON", "Ah ! dans "+arg1+" puis dans "+currentLine+" je trouve pas le ':' mais un "+QString(currentLine.at(iL))+" en "+QString::number(iL), 0);
                    break;
                }

                //on peut enregistrer dans l'objet
                JSON_Object::Type t=typeOfValue(arg2);
                if(typeOfValue(arg1)==JSON_Object::String) {
                    arg1 = arg1.left(arg1.size()-1).remove(0,1);
                    //retire les "", si c'est un nombre on s'arrangera après pour pas l'afficher
                }
                switch(t) {
                    case JSON_Object::Object:
                    case JSON_Object::Array:
                    case JSON_Object::Argument: {
                        obj->setChild(arg1, StrToJson(arg2));
                        break;
                    }
                    default://garde en tant que str
                        obj->setValue(arg1, arg2);
                }
            }
            break;
        }
        case JSON_Object::String://jamais utilisé normalement (sauf en début de fichier)
        case JSON_Object::Char: {
            obj->setValue("value", b.mid(1, b.size()-1+1-1));//sans les "" ou les ''
            break;
        }
        case JSON_Object::Number: {
            obj->setValue("value", b);
            break;
        }
        default: {
            pdeb->addLog("Load JSON", "L'objet ne commence pas par qq chose de connue, je fais comment pour le déchiffrer?", 0);
            break;
        }
    }

    return obj;
}

QString JSON::JsonToStr(JSON_Object *obj)
{
    return "JsonToStr";
}

JSON_Object::Type JSON::typeOfValue(QString value)
{
    QString charBlock=value.at(0);

    if(charBlock == "{") {
        return JSON_Object::Object;
    }
    else if (charBlock == "[") {
        return JSON_Object::Array;
    }
    else if (charBlock == "(") {
        return JSON_Object::Argument;
    }
    else if (charBlock == "\"") {
        return JSON_Object::String;
    }
    else if (charBlock == "'") {
        return JSON_Object::Char;
    }
    else {
        bool ok=true;
        value.toInt(&ok);//value ou charBlock?
        if(ok) {
            return JSON_Object::Number;
        }
        else {
            return JSON_Object::String;
        }
    }
}

QString JSON::removeUseless(QString str)
{
    int i = str.size()-1;//on retire tous les derniers char inutiles
    while(i >= 0 && (str.at(i) == " " || str.at(i) == "\n"))// || currentLine.at(iL) == ","))
        i--;//
    str.remove(i+1, str.size()-(i+1));//retire tous les derniers elements inutiles

    //on retire les premiers charactères inutiles
    if(0<str.size() && (str.at(0) == " " || str.at(0) == "\n")) {
        i=nextChar(str, 0);
        str.remove(0, i);
    }
    return str;
}



QString JSON::getBlock(QString in, int iStart)
{
    if(iStart < 0 || iStart >= in.size())
        return "";
    return in.mid(iStart, endOfBlock(in, iStart)-iStart+1);
}

int JSON::endOfBlock(QString in, int iStart)
{
    if(iStart < 0 || iStart >= in.size())
        return iStart;
    const QChar start(in.at(iStart));
    if(!block.contains(start)) {//si c'est pas un block
       //si c'est un nombre
        if(typeOfValue(start)==JSON_Object::Number) {
            int iNb=0;
            while(iNb < in.size() && typeOfValue(in.at(iNb))==JSON_Object::Number) {
                iNb++;//tant que c'est un chiffre, on continue
            }//le chiffre doit s'étendre de iStart à iNb
            return iNb-1;
        }
        return endOfElement(in, iStart);//sinon
    }
    if(block.indexOf(start) % 2==1)//si c'est une fermeture
        return iStart;

    QMap<QString, int> nbBlock;
    for(int i=0; i<block.size(); i+=2) {
        nbBlock.insert(block.at(i), 0);
    }
    nbBlock.insert(start, 1);
    nbBlock.insert("\\",0);
    int i=iStart+1;//on commence après l'ouverture du block
    while(nbBlock.value(start) > 0 && i < in.size()) {
        if(in.at(i)=="\\") {//si le actuel est un \\, on le skip et celui d'après aussi
            i+=2;
            continue;
        }
            /*if(i+1 < in.size() && in.at(i+1)=="\\") {
                nbBlock.insert("\\", (nbBlock.value("\\")+1) % 2);
            }
            else {

            }
            nbBlock.insert("\\", (nbBlock.value("\\")+1) % 2);//si c'est un 2eme \\ qui sort (du suite), on clear
        }
        else if(nbBlock.value("\\") > 0){//si c'est autre chose qu'un \\ après
            nbBlock.insert("\\", nbBlock.value("\\")-1);
            continue;//on le check pas
        }*/
        if(block.contains(in.at(i))) {
            if((nbBlock.value("\"")==1 && in.at(i)!="\"") || (nbBlock.value("'")==1 && in.at(i)!="'")) {
                i++; continue;//on est dans un str et c'est pas pour marquer la fin du str
            }
            int iT = block.indexOf(in.at(i));

            if(iT%2 == 0 && block.at(iT)==block.at(iT+1)) {//si ouvrir est pareil que fermer (change comme un booléen)
                nbBlock.insert(block.at(iT), (nbBlock.value(block.at(iT))+1) %2);//1 => 0; 0 => 1
            }
            else if(iT%2 == 1) {//on ferme
                nbBlock.insert(block.at(iT-1), nbBlock.value(block.at(iT-1))-1);
            }
            else {//on ouvre
                nbBlock.insert(block.at(iT), nbBlock.value(block.at(iT))+1);
            }
        }
        i++;
    }
    if(nbBlock.value(start) > 0) {
        pdeb->addLog("JSON", "La fin de "+QString(start)+" n'a pas été trouvé", 1);
    }
    return i-1;//dans tous les cas il sera 1 trop haut (fin de boucle)
}

int JSON::endOfElement(QString in, int iStart)
{
    //si le charactère n'est pas le début d'un block, on cherche la prochaine virgule (au meme niveau)
    int i=iStart;
    while(i < in.size() && in.at(i) != ",") {//tant qu'on a pas trouvé la prochaine virgule
        if(block.contains(in.at(i))) {//si c'est un block, on skip
            if(block.indexOf(in.at(i))%2 == 1) {//si c'est une fermeture, il y a pas de virgule après, c'etait le dernier element
                return i-1;//on passe le ccharactère avant la fermeture
            }
            i = endOfBlock(in, i);//on passe au charactère de la fermmeture du block (incrémentatation après donc il sera skip)
        }
        i++;//on incrémente ici (pour pas dépaser la size)
    }
    return i-1;//sera tjs 1 plus haut (on retire la fermeture du block/la virgule)
}

int JSON::nextChar(QString in, int i)
{
    i++;
    while(i < in.size() && (in.at(i)==" " || in.at(i)=="\n")) {
        i++;
    }
    if(i >= in.size()) i = -1;
    return i;
}
