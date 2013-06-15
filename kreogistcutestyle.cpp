#include "kreogistcutestyle.h"

KreogistCuteStyle::KreogistCuteStyle()
{
    style=QStyleFactory::create("fusion");
    style->setParent(this);
}

KreogistCuteStyle::~KreogistCuteStyle()
{
}

void KreogistCuteStyle::drawControl(ControlElement element,
                                    const QStyleOption *opt,
                                    QPainter *p,
                                    const QWidget *w) const
{
    switch(element)
    {
    case CE_MenuItem:
    {
        const QStyleOptionMenuItem *_opt_menu_item=
                qstyleoption_cast<const QStyleOptionMenuItem *>(opt);
        if(_opt_menu_item != nullptr)
            drawMenuItem(_opt_menu_item,p,w);

        break;
    }

    default:
        style->drawControl(element,opt,p,w);
        break;
    }
}


void KreogistCuteStyle::drawMenuItem(const QStyleOptionMenuItem *opt,
                                     QPainter *p,
                                     const QWidget *w) const
{
    /*switch(opt->menuItemType)
    {
    case QStyleOptionMenuItem::Normal:
    {
        int iRectX=opt->rect.x(),
            iRectY=opt->rect.y(),
            iRectW=opt->rect.width(),
            iRectH=opt->rect.height();
        QPen pen;

        if(opt->state == State_Active ||
           opt->state == State_HasFocus)
        {
            p->setBrush(opt->palette.highlight());
            p->drawRect(opt->rect);
            pen.setColor(QColor(0xf6,0xcd,0x32));
            p->setPen(pen);
            p->fillRect(opt->rect,Qt::BrushStyle::SolidPattern);
        }
        else
        {

        }
        if(opt->state)
        {
            pen.setColor(QColor(0xf6,0xcd,0x32));
            p->fillRect(opt->rect,Qt::BrushStyle::SolidPattern);
            qDebug()<<opt->text;
        }
        if(!opt->icon.isNull())
        {
            p->drawPixmap(13,
                          iRectY + (iRectH - 30)/2,
                          30,
                          30,
                          opt->icon.pixmap(30, 30));
            p->drawText(50,
                        iRectY,
                        iRectW,
                        iRectH,
                        Qt::AlignVCenter,
                        opt->text);
        }
        else
        {
            p->drawText(50,
                        iRectY,
                        iRectW,
                        iRectH,
                        Qt::AlignVCenter,
                        opt->text);
        }
        pen.setColor(QColor(0,0,0));
        p->drawLine(iRectX,iRectY+iRectH-1,iRectX+iRectW,iRectY+iRectH-1);
        break;
    }
    case QStyleOptionMenuItem::SubMenu:
    {
        int x=opt->rect.x(),
            y=opt->rect.y(),
            w=opt->rect.width(),
            h=opt->rect.height();

        if(opt->state == State_Active ||
           opt->state == State_HasFocus)
        {
            p->setBrush(opt->palette.highlight());

            p->drawRect(opt->rect);
        }

        if(!opt->icon.isNull())
        {
            p->drawPixmap(x, y, h, h,
                          opt->icon.pixmap(h, h));

            p->drawText(x + h + 2, y, w-2-h, h,
                        Qt::AlignCenter,opt->text);
        }
        else
            p->drawText(x, y, w, h,
                        Qt::AlignCenter,opt->text);

        QPen pen;
        pen.setColor(QColor(0,0,0));
        p->drawLine(x,y+h-1,x+w,y+h-1);
        break;
    }

    default:
        style->drawControl(CE_MenuItem,opt,p,w);
    }*/
    style->drawControl(CE_MenuItem,opt,p,w);
}

void KreogistCuteStyle::drawPrimitive(PrimitiveElement pe,
                                      const QStyleOption *opt,
                                      QPainter *p,
                                      const QWidget *w) const
{
    style->drawPrimitive(pe,opt,p,w);
}

void KreogistCuteStyle::drawComplexControl(ComplexControl cc,
                                           const QStyleOptionComplex *opt,
                                           QPainter *p,
                                           const QWidget *w) const
{
    style->drawComplexControl(cc,opt,p,w);
}

void KreogistCuteStyle::drawItemText(QPainter *painter,
                                     const QRect &rect,
                                     int flags,
                                     const QPalette &pal,
                                     bool enabled,
                                     const QString &text,
                                     QPalette::ColorRole textRole) const
{
    style->drawItemText(painter,rect,flags,pal,enabled,text,textRole);

}

QSize KreogistCuteStyle::sizeFromContents(ContentsType ct,
                                         const QStyleOption *opt,
                                         const QSize &contentsSize,
                                         const QWidget *widget) const
{

    switch(ct)
    {
        case CT_MenuItem:
        {
            QSize ret=style->sizeFromContents(ct,opt,contentsSize,widget);
            ret.setHeight(42);
            return ret;
            break;
        }
        default:
        {
            return style->sizeFromContents(ct,opt,contentsSize,widget);
        }
    }
}

int KreogistCuteStyle::pixelMetric(PixelMetric metric,
                                   const QStyleOption *option,
                                   const QWidget *widget) const
{
    int pIntValue = style->pixelMetric(metric,option,widget);
    switch (metric) {
    case QStyle::PM_SmallIconSize:
        pIntValue=50;
        break;
    case QStyle::PM_ButtonIconSize:
        pIntValue=25;
        break;
    case QStyle::PM_MenuHMargin:
    case QStyle::PM_MenuVMargin:
        pIntValue=0;
        break;
    case QStyle::PM_LayoutLeftMargin:
    case QStyle::PM_LayoutBottomMargin:
    case QStyle::PM_LayoutRightMargin:
    case QStyle::PM_LayoutTopMargin:
        pIntValue=0;
        break;
    default:
        break;
    }
    return pIntValue;
}

QRect KreogistCuteStyle::subElementRect(SubElement r,
                                        const QStyleOption *opt,
                                        const QWidget *widget) const
{
    return style->subElementRect(r,opt,widget);
}
QStyle::SubControl KreogistCuteStyle::hitTestComplexControl(ComplexControl cc,
                                                    const QStyleOptionComplex *opt,
                                                    const QPoint &pt,
                                                    const QWidget *w) const
{
    return style->hitTestComplexControl(cc,opt,pt,w);
}
QRect KreogistCuteStyle::subControlRect(ComplexControl cc,
                                        const QStyleOptionComplex *opt,
                                        SubControl sc,
                                        const QWidget *widget) const
{
    return style->subControlRect(cc,opt,sc,widget);
}
QPixmap KreogistCuteStyle::generatedIconPixmap(QIcon::Mode iconMode,
                                               const QPixmap &pixmap,
                                               const QStyleOption *opt) const
{
    return style->generatedIconPixmap(iconMode,pixmap,opt);
}
int KreogistCuteStyle::styleHint(StyleHint hint,
                                 const QStyleOption *option,
                                 const QWidget *widget,
                                 QStyleHintReturn *returnData) const
{
    return style->styleHint(hint,option,widget,returnData);
}
QRect KreogistCuteStyle::itemPixmapRect(const QRect &r,
                                        int flags,
                                        const QPixmap &pixmap) const
{
    return style->itemPixmapRect(r,flags,pixmap);
}
QIcon KreogistCuteStyle::standardIcon(StandardPixmap standardIcon,
                                      const QStyleOption *option,
                                      const QWidget *widget) const
{
    return style->standardIcon(standardIcon,option,widget);
}
QPixmap KreogistCuteStyle::standardPixmap(StandardPixmap standardPixmap,
                                          const QStyleOption *opt,
                                          const QWidget *widget) const
{
    return style->standardPixmap(standardPixmap,opt,widget);
}
void KreogistCuteStyle::drawItemPixmap(QPainter *painter, const QRect &rect,
                    int alignment, const QPixmap &pixmap) const
{
    return style->drawItemPixmap(painter,rect,alignment,pixmap);
}

void KreogistCuteStyle::polish(QWidget *widget)
{
    return style->polish(widget);
}
void KreogistCuteStyle::polish(QApplication *app)
{
    return style->polish(app);
}
void KreogistCuteStyle::polish(QPalette &pal)
{
    return style->polish(pal);
}
void KreogistCuteStyle::unpolish(QWidget *widget)
{
    return style->unpolish(widget);
}
void KreogistCuteStyle::unpolish(QApplication *app)
{
    return style->unpolish(app);
}
