#ifndef TEAM_COMPLIER_COMPLETELAW_H
#define TEAM_COMPLIER_COMPLETELAW_H

set <string> typeTable;

inline void initTypeTable () {
    typeTable.insert ("int");
    typeTable.insert ("float");
    typeTable.insert ("double");
    typeTable.insert ("char");
    typeTable.insert ("bool");
    typeTable.insert ("string");
};

inline void makeUpLaw () {
    int pos = 0;
    int len = programe.size () - 1;
    string tempVn = "type";
    while (pos <= len) {
        string finds = "struct ";
        pos = OriginProgamF.findPos (finds, pos);
        if (pos == -1) break;
        if (pos == 0 || programe[pos - 1] == ' ' || programe[pos - 1] == '\n');
        else {
            pos = pos + 1; continue;
        }
        int leftPos = pos + 6;
        if (leftPos > len) break;
        finds = "{";
        pos = OriginProgamF.findPos (finds, pos);
        if (pos == -1) break;
        int rightPos = pos - 1;
        string newType;
        for (int i = leftPos; i <= rightPos; i++) {
            if (programe[i] == ' ' || programe[i] == '\n') continue;
            newType += programe[i];
        }
        vector <string> tempVt;
        tempVt.push_back (newType);
        stmt newstmt = stmt (tempVn, tempVt);
        law.st.push_back (newstmt);
        cout << "type ->" << newType << endl;
        typeTable.insert (newType);
        finds = "}";
        int nextpos = OriginProgamF.findPos (finds, pos);


    }
    sort (law.st.begin (), law.st.end ());
}

#endif //TEAM_COMPLIER_COMPLETELAW_H


