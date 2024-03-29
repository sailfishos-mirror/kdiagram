/**
 * SPDX-FileCopyrightText: 2001-2015 Klaralvdalens Datakonsult AB. All rights reserved.
 *
 * This file is part of the KGantt library.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include "mainwindow.h"
#include "../common/projectmodel.h"

#include <QAction>
#include <QApplication>
#include <QComboBox>
#include <QMenuBar>
#include <QMenu>
#include <QModelIndex>
#include <QItemSelectionModel>
#include <QTreeView>
#include <QDebug>
#include <QBrush>
#include <QPainter>
#include <QPrinter>
#include <QPrintDialog>
#include <QFileDialog>
#include <QLabel>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QDialogButtonBox>

#include <KGanttGlobal>
#include <KGanttView>
#include <KGanttItemDelegate>
#include <KGanttDateTimeGrid>
#include <KGanttStyleOptionGanttItem>
#include <KGanttConstraintModel>
#include <KGanttGraphicsView>
#include <KGanttDateTimeTimeLine>

class ItemTypeComboBox : public QComboBox {
    Q_OBJECT
    Q_PROPERTY( KGantt::ItemType itemType READ itemType WRITE setItemType )
public:
    explicit ItemTypeComboBox( QWidget* parent = nullptr );

    KGantt::ItemType itemType() const;
public Q_SLOTS:
    void setItemType( KGantt::ItemType typ );
};

ItemTypeComboBox::ItemTypeComboBox( QWidget* parent )
    : QComboBox( parent )
{
    addItem( tr( "Task" ), QVariant( KGantt::TypeTask ) );
    addItem( tr( "Event" ), QVariant( KGantt::TypeEvent ) );
    addItem( tr( "Summary" ), QVariant( KGantt::TypeSummary ) );
}

KGantt::ItemType ItemTypeComboBox::itemType() const
{
    return static_cast<KGantt::ItemType>( itemData( currentIndex() ).toInt() );
}

void ItemTypeComboBox::setItemType( KGantt::ItemType typ )
{
    setCurrentIndex( typ-1 );
}

class MyItemDelegate : public KGantt::ItemDelegate {
public:
    explicit MyItemDelegate( QObject* parent = nullptr );

    /*reimp*/ QWidget* createEditor( QWidget* parent,
                                     const QStyleOptionViewItem& option,
                                     const QModelIndex& idx ) const override;
    /*reimp*/ void setEditorData( QWidget* editor, const QModelIndex& index ) const override;
    /*reimp*/ void setModelData( QWidget* editor, QAbstractItemModel* model,
                                  const QModelIndex & index ) const override;
protected:
    /*reimp*/void drawDisplay( QPainter* painter, const QStyleOptionViewItem & option,
                               const QRect& rect, const QString& text ) const override;
};

MyItemDelegate::MyItemDelegate( QObject* parent )
    : KGantt::ItemDelegate( parent )
{
}

QWidget* MyItemDelegate::createEditor( QWidget* parent,
                                       const QStyleOptionViewItem& option,
                                       const QModelIndex& idx ) const
{
    qDebug() << "MyItemDelegate::createEditor("<<parent<<idx<<")";
    if ( idx.isValid() && idx.column() == 1 )
      return new ItemTypeComboBox(parent);
    return ItemDelegate::createEditor( parent, option, idx );
}

void MyItemDelegate::setEditorData ( QWidget* editor, const QModelIndex& index ) const
{
  ItemTypeComboBox* c;
  if ( (c = qobject_cast<ItemTypeComboBox*>(editor)) && index.isValid() ) {
      c->setItemType(static_cast<KGantt::ItemType>(index.data(Qt::EditRole).toInt()));
  } else {
      ItemDelegate::setEditorData(editor,index);
  }
}

void MyItemDelegate::setModelData ( QWidget* editor, QAbstractItemModel* model,
                                  const QModelIndex & index ) const
{
  ItemTypeComboBox* c;
  if ( (c = qobject_cast<ItemTypeComboBox*>(editor)) && index.isValid() ) {
      model->setData(index,c->itemType());
  } else {
      ItemDelegate::setModelData(editor,model,index);
  }
}

void MyItemDelegate::drawDisplay( QPainter* painter, const QStyleOptionViewItem& option,
                                  const QRect& rect, const QString& text ) const
{
  //qDebug() << "MyItemDelegate::drawDisplay(" <<painter<<rect<<text<<")";
  KGantt::ItemType typ = static_cast<KGantt::ItemType>(text.toInt());
  QString str;
  switch (typ) {
      case KGantt::TypeTask: str = tr("Task"); break;
      case KGantt::TypeEvent: str = tr("Event"); break;
      case KGantt::TypeSummary: str = tr("Summary"); break;
      default: str = tr("None"); break;
  }
  ItemDelegate::drawDisplay(painter,option,rect,str);
}

///////////////////////////////////////////////////////////////////////////////
// Provide custom background and foreground
///////////////////////////////////////////////////////////////////////////////

class DateTimeGrid : public KGantt::DateTimeGrid
{
public:
    DateTimeGrid(QObject* parent = nullptr) {
        setParent(parent);
        setFreeDays( QSet<Qt::DayOfWeek>() );
        setFreeDaysBrush( QBrush( Qt::NoBrush ) );
    }
    ~DateTimeGrid() override { }

    //virtual void paintUserDefinedHeader(QPainter* painter, const QRectF& headerRect, const QRectF& exposedRect, qreal offset, const KGantt::DateTimeScaleFormatter* formatter, QWidget* widget = 0);
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void drawForeground(QPainter* painter, const QRectF& rect) override;
};

void DateTimeGrid::drawBackground(QPainter* painter, const QRectF& rect)
{
    QLinearGradient grad;
    grad.setCoordinateMode( QGradient::ObjectBoundingMode );
    grad.setStart( 0.5, 0.5 );
    grad.setFinalStop( 0.5, 0.0 );
    grad.setSpread( QGradient::ReflectSpread );
//    grad.setCenter( 0.5, 0.5 );
//    grad.setFocalPoint( 0.5, 0.5 );
//    grad.setRadius( 0.5 );
    QColor currentColor = Qt::blue;
    for ( qreal i = 0; i <= 1.0; i += 0.1 )
    {
        currentColor = currentColor.lighter( 100 + 20 * i );
        grad.setColorAt( i, currentColor );
    }
    QBrush brush( grad);
    //brush.setColor(Qt::lightGray);

    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2),
                           rect);
    painter->fillRect(r, brush);
    KGantt::DateTimeGrid::drawBackground(painter, rect);
}

void DateTimeGrid::drawForeground(QPainter* painter, const QRectF& rect)
{
    painter->save();

    QRectF r = computeRect(QDateTime::currentDateTime(),
                           QDateTime::currentDateTime().addDays(2),
                           rect);

    static QString text("Holiday");
    QFont font = painter->font();
    font.setPixelSize(r.width()/5);

    QFontMetrics fm(font);
    int width = fm.boundingRect(text).width();
    int height = fm.boundingRect(text).height();

    painter->translate(r.center());
    painter->translate(-width/2, height/2);
    painter->setFont(font);
    painter->drawText(0, 0, text);

    painter->restore();
    KGantt::DateTimeGrid::drawForeground(painter, rect);
}
/*
void DateTimeGrid::paintUserDefinedHeader( QPainter* painter, const QRectF& headerRect, const QRectF& exposedRect, qreal offset, const KGantt::DateTimeScaleFormatter* formatter, QWidget* widget)
{
    const QStyle* const style = widget ? widget->style() : QApplication::style();

    QDateTime dt = formatter->currentRangeBegin( mapToDateTime( offset + exposedRect.left() ) ).toUTC();
    qreal x = mapFromDateTime( dt );

    while ( x < exposedRect.right() + offset ) {
        const QDateTime next = formatter->nextRangeBegin( dt );
        const qreal nextx = mapFromDateTime( next );

        QStyleOptionHeader opt;
        if ( widget ) opt.init( widget );
        opt.rect = QRectF( x - offset+1, headerRect.top(), qMax( 1., nextx-x-1 ), headerRect.height() ).toAlignedRect();
        //opt.state = QStyle::State_Raised | QStyle::State_Enabled;
        opt.textAlignment = formatter->alignment();
        opt.text = formatter->text( dt );

        // use white text on black background
        opt.palette.setColor(QPalette::Window, QColor("black"));
        opt.palette.setColor(QPalette::ButtonText, QColor("white"));

        style->drawControl( QStyle::CE_Header, &opt, painter, widget );

        dt = next;
        x = nextx;
    }
}
*/
MainWindow::MainWindow( QWidget* parent )
    : QMainWindow( parent ),
      m_model( new ProjectModel( this ) ),
      m_view( new KGantt::View )
{
    m_view->setModel( m_model );
    m_view->setSelectionModel( new QItemSelectionModel(m_model));

    // slotToolsNewItem();
    m_view->leftView()->setItemDelegateForColumn( 1, new MyItemDelegate( this ) );
    m_view->leftView()->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    m_view->graphicsView()->setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );

    DateTimeGrid *grid = new DateTimeGrid(this);
    grid->timeLine()->setPen(QPen(Qt::red));
    grid->timeLine()->setOptions(KGantt::DateTimeTimeLine::UseCustomPen);
    grid->timeLine()->setInterval(5000);
    m_view->setGrid(grid);

    //QItemEditorCreatorBase *creator = new QItemEditorCreator<ItemTypeComboBox>("itemType");
    //QItemEditorFactory* factory = new QItemEditorFactory;
    //factory->registerEditor( QVariant( KGantt::TypeTask ).type(), creator );
    //m_view->itemDelegate()->setItemEditorFactory( factory );

    setCentralWidget( m_view );

    QMenuBar* mb = menuBar();

    QMenu* fileMenu = new QMenu( tr( "&File" ) );

#ifndef QT_NO_PRINTER
    fileMenu->addAction( tr( "&Save as PDF..." ), this, SLOT(slotFileSavePdf()) );
    fileMenu->addAction( tr( "&Print..." ), this, SLOT(slotFilePrint()) );
#endif

    fileMenu->addSeparator();
    fileMenu->addAction( tr( "&Quit" ), this, SLOT(slotFileQuit()) );

    mb->addMenu( fileMenu );

    QMenu* toolsMenu = new QMenu( tr( "&Tools" ) );

    toolsMenu->addAction( tr( "&New Item" ), this, SLOT(slotToolsNewItem()) );
    toolsMenu->addAction( tr( "&Add Item" ), this, SLOT(slotToolsAppendItem()) );
    toolsMenu->addAction( tr( "&Remove Item" ), this, SLOT(slotToolsRemoveItem()) );
    toolsMenu->addSeparator();
    QMenu *alignMenu = toolsMenu->addMenu( tr( "Ali&gn" ) );
    alignMenu->addAction( tr( "&Left" ), this, SLOT(slotAlignLeft()) );
    alignMenu->addAction( tr( "&Center" ), this, SLOT(slotAlignCenter()) );
    alignMenu->addAction( tr( "&Right" ), this, SLOT(slotAlignRight()) );
    alignMenu->addAction( tr( "&Hidden" ), this, SLOT(slotAlignHidden()) );
    toolsMenu->addSeparator();
    toolsMenu->addAction( tr( "&Collapse All" ), this, SLOT(slotCollapseAll()) );
    toolsMenu->addAction( tr( "&Expand All" ), this, SLOT(slotExpandAll()) );

    mb->addMenu( toolsMenu );

    /*
    slotToolsNewItem();
    slotToolsNewItem();
    slotToolsNewItem();
    for (int i = 0; i < 3; ++i) {
        m_model->setData(m_model->index(i,2,QModelIndex()), QVariant::fromValue(QDateTime::currentDateTime().addDays(i)), KGantt::StartTimeRole);
        m_model->setData(m_model->index(i,3,QModelIndex()), QVariant::fromValue(QDateTime::currentDateTime().addDays(i+1)), KGantt::EndTimeRole);
    }
    m_view->setConstraintModel(new KGantt::ConstraintModel(m_view));
    m_view->constraintModel()->addConstraint(KGantt::Constraint(m_model->index(0,0,QModelIndex()),m_model->index(1,0,QModelIndex())));
    m_view->constraintModel()->addConstraint(KGantt::Constraint(m_model->index(1,0,QModelIndex()),m_model->index(2,0,QModelIndex())));
    */
}

SavePdfDialog::SavePdfDialog(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Save as PDF"));
    QVBoxLayout *l = new QVBoxLayout(this);
    setLayout(l);

    QHBoxLayout *fileLayout = new QHBoxLayout(this);
    l->addLayout(fileLayout);
    QLabel *fileLabel = new QLabel(tr("File:"), this);
    fileLayout->addWidget(fileLabel);
    m_fileEdit = new QLineEdit(this);
    fileLabel->setBuddy(m_fileEdit);
    m_fileEdit->setText(QFileInfo(QDir::homePath(), "gantt.pdf").absoluteFilePath());
    fileLayout->addWidget(m_fileEdit);
    QPushButton *m_fileButton = new QPushButton("...", this);
    connect(m_fileButton, SIGNAL(clicked()), this, SLOT(fileButtonClicked()));
    fileLayout->addWidget(m_fileButton);

    m_rowLabels = new QCheckBox(tr("Row Header"), this);
    m_rowLabels->setChecked(true);
    l->addWidget(m_rowLabels);

    m_columnLabels = new QCheckBox(tr("Column Header"), this);
    m_columnLabels->setChecked(true);
    l->addWidget(m_columnLabels);

    QDialogButtonBox *btnBox = new QDialogButtonBox(this);
    btnBox->setStandardButtons(QDialogButtonBox::Save | QDialogButtonBox::Cancel);
    connect(btnBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(btnBox, SIGNAL(rejected()), this, SLOT(reject()));
    l->addWidget(btnBox);

    resize(QSize(400, 100).expandedTo(minimumSizeHint()));
}

void SavePdfDialog::fileButtonClicked()
{
    const QString file = QFileDialog::getSaveFileName(this, tr("Choose PDF File..."), QString(), tr("PDF files (*.pdf)"));
    if (!file.isEmpty())
        m_fileEdit->setText(file);
}

void MainWindow::slotFileSavePdf()
{
#ifndef QT_NO_PRINTER
    SavePdfDialog dialog(this);
    if (dialog.exec() != QDialog::Accepted)
        return;

    const QString file = dialog.m_fileEdit->text();
    if (file.isEmpty())
        return;

    const bool drawRowLabels = dialog.m_rowLabels->isChecked();
    const bool drawColumnLabels = dialog.m_columnLabels->isChecked();

    QPrinter printer(QPrinter::HighResolution);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setColorMode(QPrinter::Color);
    printer.setPageMargins(QMarginsF(0.2, 0.2, 0.2, 0.2), QPageLayout::Point);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(file);
    m_view->print(&printer, drawRowLabels, drawColumnLabels);
#endif
}

void MainWindow::slotFilePrint()
{
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageOrientation(QPageLayout::Landscape);
    printer.setColorMode(QPrinter::Color);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() != QDialog::Accepted)
        return;
    m_view->print(&printer);
#endif
}

void MainWindow::slotFileQuit()
{
    // TODO
    QApplication::instance()->quit();
}

void MainWindow::slotToolsNewItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        qDebug() << "MainWindow::slotToolsNewItem" << idx;
        m_model->insertRows( 0, 1, m_model->index( idx.row(),0,idx.parent() ) );
    } else {
        m_model->insertRows( 0, 1, m_view->rootIndex() );
    }
}

void MainWindow::slotToolsAppendItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        qDebug() << "MainWindow::slotToolsAppendItem" << idx;
        m_model->insertRows( m_model->rowCount( idx ), 1, m_model->index( idx.row(),0,idx.parent() ) );
    } else {
        m_model->insertRows( m_model->rowCount( m_view->rootIndex() ), 1, m_view->rootIndex() );
    }
}

void MainWindow::slotToolsRemoveItem()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        qDebug() << "MainWindow::slotToolsRemoveItem" << idx;
        m_model->removeRows( idx.row(), 1, idx.parent() );
    }
}

void MainWindow::slotCollapseAll()
{
    // don't use the treeview's collapseAll/expandAll methods but use the one provided by the
    // view cause that one will take care to update everyt6hing as needed.
    //QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    //view->collapseAll();

    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() )
        m_view->collapseAll();
}

void MainWindow::slotExpandAll()
{
    // don't use the treeview's collapseAll/expandAll methods but use the one provided by the
    // view cause that one will take care to update everyt6hing as needed.
    //QTreeView* view = qobject_cast<QTreeView*>( m_view->leftView() );
    //view->expandAll();

    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() )
        m_view->expandAll();
}

void MainWindow::slotAlignLeft()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KGantt::StyleOptionGanttItem::Left, KGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignCenter()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KGantt::StyleOptionGanttItem::Center, KGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignRight()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KGantt::StyleOptionGanttItem::Right, KGantt::TextPositionRole );
    }
}

void MainWindow::slotAlignHidden()
{
    QModelIndex idx = m_view->selectionModel()->currentIndex();
    if ( idx.isValid() ) {
        m_model->setData( idx, KGantt::StyleOptionGanttItem::Hidden, KGantt::TextPositionRole );
    }
}

#include "mainwindow.moc"
