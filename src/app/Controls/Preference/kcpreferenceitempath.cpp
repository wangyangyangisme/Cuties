#include "kcpreferenceitempath.h"

KCPreferenceItemPath::KCPreferenceItemPath(QWidget *parent) :
    KCPreferenceItemBase(parent)
{
    //Set properties
    setFixedHeight(45);
    foldAnimation->setEndFrame(45);
    showAnimation->setEndFrame(45);

    //Set language
    retranslate();

    //Set Edit Mode Layout
    editLayout=new QVBoxLayout(this);
    editLayout->setContentsMargins(5,5,5,5);
    editLayout->setSpacing(3);
    setLayout(editLayout);

    pathCaption=new QLabel(this);
    editLayout->addWidget(pathCaption);
    editLayout->addWidget(valueDisplayer);
    editLayout->addWidget(originalValueDisplayer);

    pathLayout=new QHBoxLayout();
    pathLayout->setContentsMargins(0,0,0,0);
    pathLayout->setSpacing(0);

    filePathData=new QLineEdit(this);
    browseFileAction=new QPushButton(this);
    browseFileAction->setText(browseText);
    setEditWidgetStatus(false);
    pathLayout->addWidget(filePathData, 1);
    pathLayout->addWidget(browseFileAction);

    editLayout->addLayout(pathLayout);
    editLayout->addStretch();

    showOriginalAnimation=new QTimeLine(100, this);
    showOriginalAnimation->setEndFrame(90);
    showOriginalAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(showOriginalAnimation, SIGNAL(frameChanged(int)),
            this, SLOT(setItemHeight(int)));

    hideOriginalAnimation=new QTimeLine(100, this);
    hideOriginalAnimation->setEndFrame(70);
    hideOriginalAnimation->setEasingCurve(QEasingCurve::OutCubic);
    connect(hideOriginalAnimation, SIGNAL(frameChanged(int)),
            this, SLOT(setItemHeight(int)));

    connect(browseFileAction, SIGNAL(clicked()),
            this, SLOT(getFilePath()));

    setExpandFinishedHeight(70);
}

KCPreferenceItemPath::~KCPreferenceItemPath()
{
    pathLayout->deleteLater();
}

void KCPreferenceItemPath::setOriginalValue(const QVariant &value)
{
    filePathData->setText(value.toString());
    KCPreferenceItemBase::setOriginalValue(value);
    valueDisplayer->setText(value.toString());
    setOriginalDisplayVisible(false);
    originalExpanded=false;

    connect(filePathData, SIGNAL(textChanged(QString)),
            this, SLOT(valueChangedEvent()));
}

void KCPreferenceItemPath::setPathText(const QString &value)
{
    pathText = value;
}

void KCPreferenceItemPath::setPathCaptionText(const QString &value)
{
    pathCaption->setText(value);
}

void KCPreferenceItemPath::setDialogTitle(const QString &newDialogTitle)
{
    dialogTitleText=newDialogTitle;
}

void KCPreferenceItemPath::retranslate()
{
    browseText=tr("Browse");
}

void KCPreferenceItemPath::retranslateAndSet()
{
    retranslate();
}

void KCPreferenceItemPath::setEditWidgetStatus(bool states)
{
    filePathData->setVisible(states);
    filePathData->setEnabled(states);
    browseFileAction->setVisible(states);
    browseFileAction->setEnabled(states);
}

void KCPreferenceItemPath::setWidgetValue(QVariant newWidgeValue)
{
    filePathData->setText(newWidgeValue.toString());
}

void KCPreferenceItemPath::getFilePath()
{
    QString newItemPath,
            defaultBrowsePath;
    QFileInfo currentfileInfo;
    currentfileInfo.setFile(getCurrentValue().toString());
    defaultBrowsePath=currentfileInfo.exists()?
                      currentfileInfo.absoluteDir().absolutePath():
                      ".";
    newItemPath=QFileDialog::getOpenFileName(this,
                                             dialogTitleText,
                                             defaultBrowsePath);
    if(!newItemPath.isEmpty())
    {
        setWidgetValue(newItemPath);
    }
}

QVariant KCPreferenceItemPath::getUserNewValue()
{
    return filePathData->text();
}

void KCPreferenceItemPath::refreshValueDisplay()
{
    valueDisplayer->setText(filePathData->text());
    if(valueDisplayer->text() == originalValue.toString())
    {
        setOriginalDisplayVisible(false);
        //Here, we have to set some values
        setExpandFinishedHeight(70);
        if(hideOriginalAnimation->state()!=QTimeLine::Running)
        {
            showOriginalAnimation->stop();
            hideOriginalAnimation->setStartFrame(height());
            hideOriginalAnimation->start();
        }
        originalExpanded=false;
        foldAnimation->setEndFrame(45);
        showAnimation->setEndFrame(45);
        return;
    }
    originalValueDisplayer->setText(originalValueIs +
                                    originalValue.toString());
    //Some value:
    setExpandFinishedHeight(90);
    if(!originalExpanded)
    {
        if(showOriginalAnimation->state()!=QTimeLine::Running)
        {
            hideOriginalAnimation->stop();
            showOriginalAnimation->setStartFrame(height());
            showOriginalAnimation->start();
        }
        originalExpanded=true;
    }
    foldAnimation->setEndFrame(63);
    showAnimation->setEndFrame(63);
    setOriginalDisplayVisible(true);
}
