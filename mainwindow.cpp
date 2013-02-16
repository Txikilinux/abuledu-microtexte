/** MiniTexte pour Tablette
  *
  * @warning aucun traitement d'erreur n'est pour l'instant implémenté
  * @see https://redmine.ryxeo.com/projects/abuledu-minitexte
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GPL)
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#include "mainwindow.h"
#include "ui_mainwindow.h"

// Choix des icones en fonction de la plateforme
#ifdef Q_WS_MAC
const QString rsrcPath = ":/abuledutextev1/data/images/mac";
#else
const QString rsrcPath = ":/abuledutextev1/data/images/win";
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    setAttribute(Qt::WA_QuitOnClose);

    ui->setupUi(this);

//
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_hauteurToolBar = 48;
#else
    m_hauteurToolBar = 48;
#endif

    m_abuleduMediatheque = new AbulEduMediathequeGetV1(0,"data");
    m_abuleduMediatheque->abeHideBoutonTelecharger();
    m_abuleduMediatheque->abeSetCustomBouton1(trUtf8("Insérer l'image"));
    m_abuleduMediatheque->abeSetCustomBouton1Download(true);
    m_abuleduMediatheque->abeSetDefaultView(AbulEduMediathequeGetV1::abeMediathequeThumbnails);
    connect(m_abuleduMediatheque, SIGNAL(signalMediathequeFileDownloaded(int)), this, SLOT(slotMediathequeDownload(int)));
    m_abuleduMediatheque->hide();

    m_abuledufile = QSharedPointer<AbulEduFileV1>(new AbulEduFileV1, &QObject::deleteLater);
    setCurrentFileName(m_abuledufile->abeFileGetDirectoryTemp().absolutePath() + "/document.html");

    m_abuleduFileManagerOpen = new AbulEduBoxFileManagerV1(0);
    m_abuleduFileManagerOpen->abeSetFile(m_abuledufile);
    connect(m_abuleduFileManagerOpen, SIGNAL(signalAbeFileSelected()), this, SLOT(slotOpenFile()));

    m_abuleduFileManagerSave = new AbulEduBoxFileManagerV1(0);
    m_abuleduFileManagerSave->abeSetFile(m_abuledufile);
    m_abuleduFileManagerSave->abeSetOpenOrSaveEnum(AbulEduBoxFileManagerV1::abeSave);
    connect(m_abuleduFileManagerSave, SIGNAL(signalAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)),
            this, SLOT(slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation,QString,bool)));

    // Au cas ou le widget serait un topLevelWidget()
    setWindowTitle(trUtf8("Sans nom")+"[*]");
    // On crée la barre d'icones et les QActions qui vont bien
    setupToolBarAndActions();
    // Les connexions concernant les modifications du texte et de son nom
    connect(ui->teZoneTexte->document(), SIGNAL(modificationChanged(bool)),
            m_actionSave, SLOT(setEnabled(bool)));
    connect(this, SIGNAL(alignmentRight()),   m_actionAlignRight,   SIGNAL(triggered()));
    connect(this, SIGNAL(alignmentLeft()),    m_actionAlignLeft,    SIGNAL(triggered()));
    connect(this, SIGNAL(alignmentCenter()),  m_actionAlignCenter,  SIGNAL(triggered()));
    connect(this, SIGNAL(alignmentJustify()), m_actionAlignJustify, SIGNAL(triggered()));

    connect(ui->teZoneTexte->document(), SIGNAL(modificationChanged(bool)),
            this, SLOT(setWindowModified(bool)));
    // On émet un signal inquant si le texte a été modifié
    connect(ui->teZoneTexte->document(), SIGNAL(modificationChanged(bool)),
            this, SIGNAL(somethingHasChangedInText(bool)));
    //    // Le curseur a changé lors d'une opération d'édition
    //    connect(ui->teZoneTexte->document(), SIGNAL(cursorPositionChanged(QTextCursor)),
    //            this, SLOT(cursorMoved(QTextCursor)));
    // Le curseur a été déplacé
    connect(ui->teZoneTexte, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorMoved()));

    m_isNewFile = true;

//    ui->toolBar->addWidget(ui->widgetTextEditor->abeTexteGetToolBar());

    QWidget *spacerWidget = new QWidget(ui->toolBar);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    QAction *actionQuit = new QAction(QIcon(":/abuledutextev1/fermer-48"),trUtf8("Quit"), this);
    connect(actionQuit,SIGNAL(triggered()),this,SLOT(close()));

    ui->toolBar->addWidget(spacerWidget);
    ui->toolBar->addAction(actionQuit);
    resize(1024,600);

    setWindowTitle(trUtf8("Mini traitement de texte pour AbulÉdu - Fichier Sans nom")+"[*]");

    //ui->teZoneTexte->setFocus();
    //    setFixedSize(1024, 600);

    m_picoLecteur = new AbulEduPicottsV1(4);

}

MainWindow::~MainWindow()
{
    delete ui;

    delete m_abuleduMediatheque;
    delete m_abuleduFileManagerOpen;
    delete m_abuleduFileManagerSave;
    m_abuledufile->abeClean();
}

QTextDocument *MainWindow::abeTexteGetDocument()
{
    return ui->teZoneTexte->document();
}

QToolBar *MainWindow::abeTexteGetToolBar()
{
    return tb;
}

void MainWindow::abeTexteSetFontFamily(QString fontFamily)
{
    //    m_comboFont->setCurrentFont(QFont(fontFamily));
    setTextFamily();
}

QString MainWindow::abeTexteGetFontFamily()
{
    //    return m_comboFont->font().family();
}

void MainWindow::abeTexteSetFontSize(int taille)
{
    /** @todo Tester taille et l'adapter en fonction des tailles disponibles dans la combobox m_comboSize */
//    m_comboSize->setCurrentIndex(m_comboSize->findText(QString::number(taille)));
    setTextSize(QString::number(taille));
}

void MainWindow::abeTexteSetAlignment(Qt::Alignment align)
{
    if(align.testFlag(Qt::AlignRight))
        emit alignmentRight();
    else if(align.testFlag(Qt::AlignLeft))
        emit alignmentLeft();
    else if(align.testFlag(Qt::AlignHCenter))
        emit alignmentCenter();
    else if(align.testFlag(Qt::AlignJustify))
        emit alignmentCenter();
    updateActions(ui->teZoneTexte->textCursor().charFormat()); // Met le bouton concerné à jour
}

void MainWindow::abeTexteSetBold(bool onOff)
{
    m_actionTextBold->setChecked(onOff);
    setTextFormat();
}

void MainWindow::abeTexteSetItalic(bool onOff)
{
    m_actionTextItalic->setChecked(onOff);
    setTextFormat();
}

void MainWindow::abeTexteSetUnderline(bool onOff)
{
    m_actionTextUnderline->setChecked(onOff);
    setTextFormat();
}

void MainWindow::abeTexteToolBarSetVisible(bool ouiNon)
{
    tb->setVisible(ouiNon);
}

bool MainWindow::abeTexteToolBarIsVisible()
{
    return tb->isVisible();
}

void MainWindow::setTextFormat()
{
    // On crée le format à appliquer
    QTextCharFormat fmt;
    fmt.setFontWeight(m_actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
    fmt.setFontItalic(m_actionTextItalic->isChecked());
    fmt.setFontUnderline(m_actionTextUnderline->isChecked());
    // On l'applique
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::setTextAlign(QAction *action)
{
    // On applique le bon alignement pour le paragraphe sous le curseur
    if (action == m_actionAlignLeft)
        ui->teZoneTexte->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute); // Toujours à gauche même en cas de RTL
    else if (action == m_actionAlignCenter)
        ui->teZoneTexte->setAlignment(Qt::AlignHCenter);
    else if (action == m_actionAlignRight)
        ui->teZoneTexte->setAlignment(Qt::AlignRight | Qt::AlignAbsolute); // Toujours à droite même en cas de RTL
    else if (action == m_actionAlignJustify)
        ui->teZoneTexte->setAlignment(Qt::AlignJustify);
}

void MainWindow::setTextFamily()
{
    QString f = sender()->objectName();
    qDebug() << " Fonte : " << f;
    // On applique le format de font sélectionnée
    QTextCharFormat fmt;
    fmt.setFontFamily(f);
    mergeFormatOnWordOrSelection(fmt);
    setTextSize("20");
//    ui->teZoneTexte->setFocus();

    //Espacement vertical different
    QTextBlockFormat format;
    format.setLineHeight(sender()->property("interligne").toInt(), QTextBlockFormat::ProportionalHeight);
    QTextCursor curseur = ui->teZoneTexte->textCursor();
    curseur.setBlockFormat(format);
}

void MainWindow::setTextSize(const QString &p)
{
    // On applique la taille de font sélectionnée
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        mergeFormatOnWordOrSelection(fmt);
    }
}

void MainWindow::setTextColor()
{
    // Applique la couleur sélectionnée au texte
    QColor col = QColorDialog::getColor(ui->teZoneTexte->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);

    // On met à jour l'icone dans la barre de boutons
    colorChanged(col);
}

void MainWindow::colorChanged(const QColor &couleur)
{
    QPixmap pix(16, 16);
    pix.fill(couleur);
    m_actionTextColor->setIcon(pix);
}

void MainWindow::setupToolBarAndActions()
{
    // Création de la Barre de boutons
    tb = ui->toolBar;
    tb->setToolButtonStyle(Qt::ToolButtonIconOnly);
    tb->setFixedHeight(m_hauteurToolBar);
//    tb->setWindowTitle(trUtf8("&Édition"));

    /** @todo utiliser le thème abuledu pour les icones des toolboutons
      * Pour l'instant, on utilise l'icone du thème, sinon celle du fichier de ressources
      */
    // Ouvrir un fichier
    m_actionOpen = new QAction(QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png")),
                               trUtf8("&Ouvrir"), this);
    m_actionOpen->setObjectName("open");
    m_actionOpen->setShortcut(QKeySequence::Open);
    connect(m_actionOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
    m_actionOpen->setEnabled(true);
    tb->addAction(m_actionOpen);

    // Sauvegarde du texte
    m_actionSave = new QAction(QIcon::fromTheme("document-save", QIcon(rsrcPath + "/filesave.png")),
                               trUtf8("&Enregistrer"), this);
    m_actionSave->setObjectName("save");
    m_actionSave->setShortcut(QKeySequence::Save);
    connect(m_actionSave, SIGNAL(triggered()), this, SLOT(fileSave()));
    m_actionSave->setEnabled(true);
    tb->addAction(m_actionSave);


    m_actionPrint = new QAction(QIcon::fromTheme("document-print", QIcon(rsrcPath + "/fileprint.png")),
                                trUtf8("&Imprimer..."), this);
    m_actionPrint->setObjectName("print");
    m_actionPrint->setPriority(QAction::LowPriority);
    m_actionPrint->setShortcut(QKeySequence::Print);
    connect(m_actionPrint, SIGNAL(triggered()), this, SLOT(filePrint()));
    tb->addAction(m_actionPrint);

    tb->addSeparator();

    // Formatage des caractères
    m_actionTextBold = new QAction(QIcon::fromTheme("format-text-bold", QIcon(rsrcPath + "/textbold.png")),
                                   trUtf8("&Gras"), this);
    m_actionTextBold->setObjectName("bold");
    m_actionTextBold->setShortcut(Qt::CTRL + Qt::Key_B);
    m_actionTextBold->setPriority(QAction::LowPriority);
    QFont bold;
    bold.setBold(true);
    m_actionTextBold->setFont(bold);
    connect(m_actionTextBold, SIGNAL(triggered()), this, SLOT(setTextFormat()));
    tb->addAction(m_actionTextBold);
    m_actionTextBold->setCheckable(true);

    m_actionTextItalic = new QAction(QIcon::fromTheme("format-text-italic", QIcon(rsrcPath + "/textitalic.png")),
                                     trUtf8("&Italique"), this);
    m_actionTextItalic->setObjectName("italique");
    m_actionTextItalic->setPriority(QAction::LowPriority);
    m_actionTextItalic->setShortcut(Qt::CTRL + Qt::Key_I);
    QFont italic;
    italic.setItalic(true);
    m_actionTextItalic->setFont(italic);
    connect(m_actionTextItalic, SIGNAL(triggered()), this, SLOT(setTextFormat()));
    tb->addAction(m_actionTextItalic);
    m_actionTextItalic->setCheckable(true);

    m_actionTextUnderline = new QAction(QIcon::fromTheme("format-text-underline", QIcon(rsrcPath + "/textunder.png")),
                                        trUtf8("&Souligné"), this);
    m_actionTextUnderline->setObjectName("underline");
    m_actionTextUnderline->setShortcut(Qt::CTRL + Qt::Key_U);
    m_actionTextUnderline->setPriority(QAction::LowPriority);
    QFont underline;
    underline.setUnderline(true);
    m_actionTextUnderline->setFont(underline);
    connect(m_actionTextUnderline, SIGNAL(triggered()), this, SLOT(setTextFormat()));
    tb->addAction(m_actionTextUnderline);
    m_actionTextUnderline->setCheckable(true);

    tb->addSeparator();

    // Alignement des paragraphes
    m_alignActions = new QActionGroup(this);
    m_alignActions->setObjectName("groupalign");
    connect(m_alignActions, SIGNAL(triggered(QAction*)), this, SLOT(setTextAlign(QAction*)));

    // On modifie la position des icones en fonction du sens du texte LTR ou RTL
    if (QApplication::isLeftToRight()) {
        m_actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")), trUtf8("À gauc&he"), m_alignActions);
        m_actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), trUtf8("Au c&entre"), m_alignActions);
        m_actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), trUtf8("À d&roite"), m_alignActions);
    } else {
        m_actionAlignRight = new QAction(QIcon::fromTheme("format-justify-right", QIcon(rsrcPath + "/textright.png")), trUtf8("&À d&roite"), m_alignActions);
        m_actionAlignCenter = new QAction(QIcon::fromTheme("format-justify-center", QIcon(rsrcPath + "/textcenter.png")), trUtf8("Au c&entre"), m_alignActions);
        m_actionAlignLeft = new QAction(QIcon::fromTheme("format-justify-left", QIcon(rsrcPath + "/textleft.png")), trUtf8("À gauc&he"), m_alignActions);
    }
    m_actionAlignJustify = new QAction(QIcon::fromTheme("format-justify-fill", QIcon(rsrcPath + "/textjustify.png")), trUtf8("&Justifié"), m_alignActions);

    m_actionAlignLeft->setShortcut(Qt::CTRL + Qt::Key_L);
    m_actionAlignLeft->setObjectName("alignleft");
    m_actionAlignLeft->setCheckable(true);
    m_actionAlignLeft->setPriority(QAction::LowPriority);
    m_actionAlignCenter->setShortcut(Qt::CTRL + Qt::Key_E);
    m_actionAlignCenter->setObjectName("aligncenter");
    m_actionAlignCenter->setCheckable(true);
    m_actionAlignCenter->setPriority(QAction::LowPriority);
    m_actionAlignRight->setShortcut(Qt::CTRL + Qt::Key_R);
    m_actionAlignRight->setObjectName("alignright");
    m_actionAlignRight->setCheckable(true);
    m_actionAlignRight->setPriority(QAction::LowPriority);
    m_actionAlignJustify->setShortcut(Qt::CTRL + Qt::Key_J);
    m_actionAlignJustify->setObjectName("alignjustify");
    m_actionAlignJustify->setCheckable(true);
    m_actionAlignJustify->setPriority(QAction::LowPriority);

    tb->addActions(m_alignActions->actions());

    tb->addSeparator();

    QFontDatabase fonts;
    if( ! fonts.addApplicationFont(":/abuledutextev1/Seyes")) {
        qDebug() << "Erreur sur :/fonts/SEYESBDE.TTF";
    }
    if( ! fonts.addApplicationFont(":/abuledutextev1/Crayon")) {
        qDebug() << "Erreur sur :/fonts/CRAYONE.TTF";
    }
    if( ! fonts.addApplicationFont(":/abuledutextev1/Plume")) {
        qDebug() << "Erreur sur :/fonts/PLUMBAE.TTF";
    }

    //Pour tablettes, je préfère des boutons ...
    m_btnFontAndika = new AbulEduFlatBoutonV1();
    m_btnFontAndika->setText("Andika");
    m_btnFontAndika->setFont(QFont("andika",14));
    m_btnFontAndika->setObjectName("andika");
    m_btnFontAndika->setProperty("interligne",100);
    tb->addWidget(m_btnFontAndika);
    connect(m_btnFontAndika, SIGNAL(clicked()), this, SLOT(setTextFamily()));

    m_btnFontSeyes= new AbulEduFlatBoutonV1();
    m_btnFontSeyes->setText("Seyes");
    m_btnFontSeyes->setFont(QFont("SeyesBDE",16));
    m_btnFontSeyes->setObjectName("SeyesBDE");
    m_btnFontSeyes->setProperty("interligne",200);
    tb->addWidget(m_btnFontSeyes);
    connect(m_btnFontSeyes, SIGNAL(clicked()), this, SLOT(setTextFamily()));

    m_btnFontCrayon= new AbulEduFlatBoutonV1();
    m_btnFontCrayon->setText("Crayon");
    m_btnFontCrayon->setFont(QFont("CrayonE",16));
    m_btnFontCrayon->setObjectName("CrayonE");
    m_btnFontCrayon->setProperty("interligne",100);
    tb->addWidget(m_btnFontCrayon);
    connect(m_btnFontCrayon, SIGNAL(clicked()), this, SLOT(setTextFamily()));

    m_btnFontPlume= new AbulEduFlatBoutonV1();
    m_btnFontPlume->setText("Plume");
    m_btnFontPlume->setFont(QFont("PlumBAE",16));
    m_btnFontPlume->setObjectName("PlumBAE");
    m_btnFontPlume->setProperty("interligne",100);
    tb->addWidget(m_btnFontPlume);
    connect(m_btnFontPlume, SIGNAL(clicked()), this, SLOT(setTextFamily()));

    // Les actions concernant le choix de la police création de la combobox
//    m_comboFont = new QComboBox(tb);
//    m_comboFont->setObjectName("combofont");
//    m_comboFont->addItem("Andika");
//    m_comboFont->addItem("CrayonE");
//    m_comboFont->addItem("PlumBAE");
//    m_comboFont->addItem("SeyesBDE");
//    tb->addWidget(m_comboFont);
//    m_comboFont->setEditable(false);
//    connect(m_comboFont, SIGNAL(activated(QString)),
//            this, SLOT(setTextFamily(QString)));
//    // La taille de la police, création de la combobox
//    m_comboSize = new QComboBox(tb);
//    m_comboSize->setObjectName("comboSize");
//    tb->addWidget(m_comboSize);
//    m_comboSize->setEditable(true);

//    QFontDatabase db;
//    foreach(int size, db.standardSizes())
//        m_comboSize->addItem(QString::number(size)); // On ajoute dans la combobox les tailles valides

//    connect(m_comboSize, SIGNAL(activated(QString)),
//            this, SLOT(setTextSize(QString)));
//    m_comboSize->setCurrentIndex(m_comboSize->findText(QString::number(QApplication::font()
//                                                                       .pointSize())));

    // Création de l'icone de la couleur sélectionnée
    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    m_actionTextColor = new QAction(pix, trUtf8("&Couleur..."), this);
    m_actionTextColor->setObjectName("color");
    connect(m_actionTextColor, SIGNAL(triggered()), this, SLOT(setTextColor()));
    tb->addAction(m_actionTextColor);

    m_actionImageFromData = new QAction(QIcon::fromTheme("image-from-data", QIcon(":/abuledutextev1/data/images/cloud.svg")), trUtf8("Insérer une image"), this);
    m_actionImageFromData->setObjectName("mediatheque-data");

#ifdef __ABULEDUTABLETTEV1__MODE__
    connect(m_actionImageFromData, SIGNAL(triggered()), m_abuleduMediatheque, SLOT(showFullScreen()));
#else
    connect(m_actionImageFromData, SIGNAL(triggered()), m_abuleduMediatheque, SLOT(show()));
#endif
    tb->addAction(m_actionImageFromData);

}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = ui->teZoneTexte->textCursor();
    if (!cursor.hasSelection())
        // Si pas de sélection, on utilise le mot sous le curseur
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    ui->teZoneTexte->mergeCurrentCharFormat(format);
}

bool MainWindow::fileSave()
{
    setCurrentFileName(m_abuledufile->abeFileGetDirectoryTemp().absolutePath() + "/document.html");

    qDebug() << "Ecriture dans le fichier " << m_fileName;

    //
    QFileInfo fi(m_fileName);

    QTextDocumentWriter writer(fi.absoluteFilePath(),"HTML");
    bool success = writer.write(ui->teZoneTexte->document());
    if (success)
        ui->teZoneTexte->document()->setModified(false);

    //Le 1er fichier de la liste, c'est le fichier document maitre html
    QStringList liste(m_fileName);
    //Parcours du repertoire data pour enquiller tous les autres fichiers
    QDir dir(fi.absolutePath() + "/data/");
    //Attention a ne pas prendre le repertoire "." et ".."
    foreach(QFileInfo fileInfo, dir.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot)) {
        if(fileInfo.isFile()) {
            liste << fileInfo.absoluteFilePath();
        }
    }

    m_abuledufile->abeFileExportPrepare(liste, fi.absolutePath(), "abe");

    if (m_isNewFile) {
        fileSaveAs(); // Ouverture du sélecteur de fichier
    }

//    if(m_abuledufile->abeFileSave(m_abuledufile->abeFileGetFileName().absoluteFilePath(),
//                               liste,
//                               fi.absolutePath(),
//                               QString("abe"))) {
//        //rien à dire
//        qDebug() << "Sauvegarde ok, fichier : " << m_abuledufile->abeFileGetFileName().absoluteFilePath();
//    }
//    else {
//        AbulEduMessageBoxV1 *alertBox=new AbulEduMessageBoxV1(trUtf8("Erreur de sauvegarde"),trUtf8("ERREUR: Le document n'a pas été sauvegardé."));
//        alertBox->show();
//    }
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_abuleduFileManagerSave->showFullScreen();
#else
    m_abuleduFileManagerSave->show();
#endif

    return success;
}

bool MainWindow::fileSaveAs()
{
    m_isNewFile = false;
    fileSave();
}
void MainWindow::setCurrentFileName(const QString &fileName)
{
    m_fileName = fileName;
    // Comme le nom vient de changer, c'est que le fichier vient d'être crée ou vient d'être sauvegardé
    ui->teZoneTexte->document()->setModified(false);

    QDir rep(m_abuledufile->abeFileGetDirectoryTemp().absolutePath());
    if(!rep.exists()) {
        rep.mkpath(m_abuledufile->abeFileGetDirectoryTemp().absolutePath());
    }

    QString shownName;
    if (fileName.isEmpty()) {
        shownName = trUtf8("Sans nom") +"[*]";
    }
    else {
        shownName = QFileInfo(m_fileName).fileName() + "[*]";
    }

    // Au cas ou le widget serait un topLevelWidget()
    setWindowTitle(shownName);
    // On émet un signal avec le nom du fichier suivi de [*] pour affichage dans titre de fenêtre
    emit fileNameHasChanged(shownName);
}


bool MainWindow::abeTexteInsertImage(QString cheminImage, qreal width, qreal height, QTextFrameFormat::Position position, QString name)
{
    QFile fichier(cheminImage);
    if(!fichier.exists()){
        qDebug()<<__PRETTY_FUNCTION__<<"ligne"<<__LINE__<<"Le fichier n'existe pas"<<cheminImage;
        return false;
    }
    else
    {
        if(name == "")
        {
            name = cheminImage;
        }
        QImage image(cheminImage);
        if(width == 0 || height == 0)
        {
            width = image.width();
            height = image.height();
        }
        else
        {
            image = image.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        ui->teZoneTexte->document()->addResource(QTextDocument::ImageResource, QUrl(name), image);

        /** Changer la méthode d'insertion pour insérer une QTetxtFrame dans laquelle on insérerait une image
          * ce qui devrait donner plus de souplesse
          * peut-être :-D
          */
        QTextImageFormat *imageFmt = new QTextImageFormat();
        imageFmt->setName(name);
        //        imageFmt->setWidth(width);
        //        imageFmt->setHeight(height);

        ui->teZoneTexte->textCursor().insertImage(*imageFmt, position);
        return true;
    }
}

void MainWindow::filePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog *dlg = new QPrintDialog(&printer, this);
    if (ui->teZoneTexte->textCursor().hasSelection())
        dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    dlg->setWindowTitle(trUtf8("Imprimer le document"));
    if (dlg->exec() == QDialog::Accepted) {
        ui->teZoneTexte->print(&printer);
    }
    delete dlg;
#endif
}

//void MainWindow::cursorMoved(QTextCursor curseur)
//{


//}

void MainWindow::cursorMoved()
{
    //    if(m_localDebug)qDebug()<<__PRETTY_FUNCTION__<<"ligne"<<__LINE__;
    updateActions(ui->teZoneTexte->textCursor().charFormat());
}

void MainWindow::updateActions(QTextCharFormat fmt)
{
    // On actualise les toolButtons ou plutôt les QActions sous-jacentes en fonction du formatage du texte
    m_actionTextBold->blockSignals(true);
    m_actionTextBold->setChecked(fmt.fontWeight() >= QFont::Bold);
    m_actionTextBold->blockSignals(false);

    m_actionTextItalic->blockSignals(true);
    m_actionTextItalic->setChecked(fmt.fontItalic());
    m_actionTextItalic->blockSignals(false);

    m_actionTextUnderline->blockSignals(true);
    m_actionTextUnderline->setChecked(fmt.fontUnderline());
    m_actionTextUnderline->blockSignals(false);

    //    m_actionTextColor->blockSignals(true);
    colorChanged(fmt.foreground().color());
    //    m_actionTextColor->blockSignals(false);

    m_actionAlignLeft->blockSignals(true);
    m_actionAlignLeft->setChecked(ui->teZoneTexte->alignment().testFlag(Qt::AlignLeft));
    m_actionAlignLeft->blockSignals(false);

    m_actionAlignCenter->blockSignals(true);
    m_actionAlignCenter->setChecked(ui->teZoneTexte->alignment().testFlag(Qt::AlignHCenter));
    m_actionAlignCenter->blockSignals(false);

    m_actionAlignRight->blockSignals(true);
    m_actionAlignRight->setChecked(ui->teZoneTexte->alignment().testFlag(Qt::AlignRight));
    m_actionAlignRight->blockSignals(false);

    m_actionAlignJustify->blockSignals(true);
    m_actionAlignJustify->setChecked(ui->teZoneTexte->alignment().testFlag(Qt::AlignJustify));
    m_actionAlignJustify->blockSignals(false);

//    m_comboFont->blockSignals(true);
//    //    m_comboFont->setCurrentFont(fmt.font());
//    m_comboFont->blockSignals(false);

//    m_comboSize->blockSignals(true);
//    m_comboSize->setCurrentIndex(m_comboSize->findText(QString::number(fmt.font().pointSize())));
//    m_comboSize->blockSignals(false);
}

void MainWindow::slotMediathequeDownload(int code)
{
    QString file = m_abuleduMediatheque->abeGetFile()->abeFileGetContent(0).absoluteFilePath();
    QString filename = m_abuleduMediatheque->abeGetFile()->abeFileGetContent(0).baseName() + ".png";

    qDebug() << "  slotMediathequeDownload : " << file << " et " << filename;

    QUrl Uri ( QString ( "mydata://data/%1" ).arg ( filename ) );
    QImage image = QImageReader ( file ).read().scaledToWidth(150,Qt::SmoothTransformation);

    QFileInfo fi(m_fileName);
    QString imageDest = QString("%1/data/%2").arg(fi.absolutePath()).arg(filename);
//    Uri.setUrl(imageDest);

    QTextDocument * textDocument = ui->teZoneTexte->document();
    textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
    QTextCursor cursor = ui->teZoneTexte->textCursor();
    QTextImageFormat imageFormat;
    imageFormat.setWidth( image.width() );
    imageFormat.setHeight( image.height() );

    QDir rep(fi.absolutePath() + "/data/");
    if(!rep.exists()) {
        rep.mkpath(fi.absolutePath() + "/data/");
    }
    if(!image.save(imageDest)) {
//        qDebug() << "******* ERREUR de sauvegarde de " << imageDest;
    }
    qDebug() << "Sauvegarde de l'image dans " << imageDest;

    imageFormat.setName(Uri.toString());
    cursor.insertImage(imageFormat);
    cursor.insertText("\n");

    //Les sources et l'auteur (?)
    QTextListFormat listFormat;
    cursor.insertList(listFormat);
    QTextCharFormat fmt;
    fmt.setFontItalic(true);
    cursor.insertText("Source: " + m_abuleduMediatheque->abeGetFile()->abeFileGetIdentifier() + "\n",fmt);
    cursor.insertText("Auteur: " + m_abuleduMediatheque->abeGetFile()->abeFileGetCreator(),fmt);

    //Retour normal
    QTextBlockFormat blockFormat;
    fmt.setFontItalic(false);
    cursor.insertBlock(blockFormat,fmt);

    m_abuleduMediatheque->hide();
}

void MainWindow::fileOpen()
{
#ifdef __ABULEDUTABLETTEV1__MODE__
    m_abuleduFileManagerOpen->showFullScreen();
#else
    m_abuleduFileManagerOpen->show();
#endif
}

void MainWindow::slotOpenFile()
{
    qDebug() << "Ouverture du fichier " << m_abuledufile->abeFileGetFileName().absoluteFilePath();
    setCurrentFileName(m_abuledufile->abeFileGetContent(0).absoluteFilePath());
    m_abuleduFileManagerOpen->hide();

    // ==============================================================================
    // lecture du fichier html
    QFile  htmlFile(m_fileName);
    if (!htmlFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        return;
    }

    QString htmlContent;
    QTextStream in(&htmlFile);
    while (!in.atEnd()) {
        htmlContent.append(in.readLine());
    }

    QTextDocument *document = new QTextDocument();
    document->setHtml(htmlContent);
    ui->teZoneTexte->setDocument(document);


    // ==============================================================================
    // chargement des ressources dans le textDocument...
    QTextDocument * textDocument = ui->teZoneTexte->document();
    QStringList liste = m_abuledufile->abeFileGetFileList();
    for(int i = 0; i < liste.size(); i++) {
        QFileInfo fi(liste.at(i));
        if(fi.suffix() == "png") {
            QUrl Uri ( QString ( "mydata://data/%1" ).arg ( fi.fileName() ) );
            QImage image = QImageReader(fi.absoluteFilePath()).read();
            textDocument->addResource( QTextDocument::ImageResource, Uri, QVariant ( image ) );
            qDebug() << " ++ " << fi.absoluteFilePath() << " en tant que " << Uri;
        }
    }

    ui->teZoneTexte->update();
    qDebug() << document->toHtml();
}

void MainWindow::slotAbeFileSaved(AbulEduBoxFileManagerV1::enumAbulEduBoxFileManagerSavingLocation box, QString fileName, bool etat)
{
    qDebug() << "slotAbeFileSaved : " << fileName << " et " << etat;
}

void MainWindow::on_btnLire_clicked()
{
    QString txt = QString("<break time=\"1s\"><speed level=\"80\"><volume level=\"100\">%1</speed>").arg(ui->teZoneTexte->toPlainText());
    m_picoLecteur->abePicoPlay(txt);
    ui->btnPause->setText(trUtf8("Pause"));
}

void MainWindow::on_btnPause_clicked()
{
    if(ui->btnPause->text() == trUtf8("Continuer")) {
        m_picoLecteur->abePicoResume();
        ui->btnPause->setText(trUtf8("Pause"));
    }
    else {
        m_picoLecteur->abePicoPause();
        ui->btnPause->setText(trUtf8("Continuer"));
    }
}


void MainWindow::on_btnStop_clicked()
{
    m_picoLecteur->abePicoStop();
}
