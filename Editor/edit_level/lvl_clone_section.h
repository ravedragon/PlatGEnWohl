#ifndef LVL_CLONE_SECTION_H
#define LVL_CLONE_SECTION_H

#include <QDialog>
#include "level_edit.h"

namespace Ui {
class LvlCloneSection;
}

class LvlCloneSection : public QDialog
{
    Q_OBJECT

public:
    explicit LvlCloneSection(QWidget *parent = 0);
    ~LvlCloneSection();
    void addLevelList(QList<leveledit* > _levels, leveledit *active = NULL);

    int clone_margin;
    leveledit* clone_source;
    int clone_source_id;
    leveledit* clone_target;
    int clone_target_id;

private slots:
    void on_FileList_src_currentIndexChanged(int index);
    void on_FileList_dst_currentIndexChanged(int index);
    void on_buttonBox_accepted();

private:
    QList<leveledit* > levels;
    Ui::LvlCloneSection *ui;
};

#endif // LVL_CLONE_SECTION_H
