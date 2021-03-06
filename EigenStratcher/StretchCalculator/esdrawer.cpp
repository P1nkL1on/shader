#include "esdrawer.h"
#include "iostream"
using namespace std;
using namespace Eigen;

void EsDrawer::setScale(const double scaling)
{
    m_scaling = scaling;
}

QColor mixColor(double k, const QColor original){
    return QColor(int(original.red() * k),int(original.green() * k),int(original.blue() * k));
}

void EsDrawer::drawModel(QPainter *painter, const EsModel &model, const QColor clr) const
{
    vector<int> res;
    drawModel(painter, model, res, clr);
}

void EsDrawer::drawModel(QPainter *painter, const EsModel &model,
                         std::vector<int> &resVertices, const QColor clr) const
{
    const auto v = model.v();
    const auto vt = model.vt();
    const auto s = model.s();
    const auto st = model.st();

    if (st.length() % 3 != 0)
        return;
    for (int i = 0; i < st.length(); i+=3)
        drawTriangleUVmonocolor(painter, vt[st[i]], vt[st[i + 1]], vt[st[i + 2]], 4 - i / 3,mixColor(.5 + .5 * i / st.length(), clr));

    drawSystemG(painter, 5.0);
    drawSystemUV(painter);

    resVertices = std::vector<int>(s.length() * 2);
    std::fill(resVertices.begin(), resVertices.end(), 0.0);

    const int polygonCount = s.length() / 3;
    for (int i = 0; i < s.length(); i+=3){
        drawTriangleGmonocolor(painter, v[s[i]], v[s[i + 1]], v[s[i + 2]], 2, mixColor(.5 + .5 * (i/3) / polygonCount, clr));
        for (int j = 0; j < 3; ++j){
            resVertices[(i + j) * 2] = translateVec3(v[s[i + j]]).x();
            resVertices[(i + j) * 2 + 1] = translateVec3(v[s[i + j]]).y();
        }
    }
}

void EsDrawer::drawSystemG(QPainter *painter, const double scale) const
{
    Matrix3d worldCoords = Matrix3d::Identity() * scale;

    Vector3d zero = Vector3d::Zero();
    Vector3d zeroX = worldCoords.col(0);
    Vector3d zeroY = worldCoords.col(1);
    Vector3d zeroZ = worldCoords.col(2);
    drawLine(painter, zero, zeroX, Qt::red);
    drawLine(painter, zero, zeroY, Qt::green);
    drawLine(painter, zero, zeroZ, Qt::blue);
}

void EsDrawer::drawSystemUV(QPainter *painter) const
{
    Matrix2d worldCoords = Matrix2d::Identity();

    Vector2d zero = Vector2d::Zero();
    Vector2d ones = Vector2d::Ones();

    const Vector2d zeroX = worldCoords.col(0);
    const Vector2d zeroY = worldCoords.col(1);

    drawLine(painter, zero, zeroX * 1.2, Qt::red, 3);
    drawLine(painter, zero, zeroY * 1.2, Qt::green, 3);
    drawLine(painter, ones, zeroY, Qt::red, 1);
    drawLine(painter, ones, zeroX, Qt::green, 1);
}

void EsDrawer::drawLine(QPainter *painter, const Vector2d &v1, const Vector2d &v2, const QColor &color, const float width) const
{
    const auto from = translateVec2(v1, true);
    const auto to = translateVec2(v2, true);
    painter->setPen(QPen(color, width));
//    cout << from.x() <<" " << from.y()<<" "  << to.x()<<" "  << to.y() << endl;
    painter->drawLine(from.x(), from.y(), to.x(), to.y());
}

void EsDrawer::drawLine(QPainter *painter, const Vector3d &v1, const Vector3d &v2, const QColor &color, const float width) const
{
    const auto from = translateVec3(v1);
    const auto to = translateVec3(v2);
    painter->setPen(QPen(color, width));
    painter->drawLine(from.x(), from.y(), to.x(), to.y());
}

//drawTriangleG(painter, v1,v2,v3);
void EsDrawer::drawTriangleG(QPainter *painter,
                             const Vector3d &v1, const Vector3d &v2, const Vector3d &v3,
                             const float lineWidth, const QVector<QColor> triangleColors) const
{
    if (triangleColors.length() != 3)
        return;
    QVector<QVector2D> planeC = { translateVec3(v1), translateVec3(v2), translateVec3(v3) };
    for (int i = 0; i < 3; ++i){
        Vector3d proect = (i == 0)? v1 : ((i == 1)? v2 : v3);

        Vector3d from = proect;
        from(1, 0) = 0.0;
        Vector3d from2 = from;
        from2(0,0) = 0.0;
        Vector3d from3 = from;
        from3(2,0) = 0.0;
        drawLine(painter, proect, from, Qt::green);
        drawLine(painter, from2, from, Qt::red);
        drawLine(painter, from3, from, Qt::blue);

    }
    drawTriDotTriangle(painter, planeC, lineWidth, triangleColors);
}

void EsDrawer::drawTriangleGmonocolor(QPainter *painter, const Vector3d &v1, const Vector3d &v2, const Vector3d &v3, const float lineWidth, const QColor triangleColor) const
{
    drawTriangleG(painter, v1,v2,v3, lineWidth, {triangleColor, triangleColor, triangleColor});
}

void EsDrawer::drawTriangleUV(QPainter *painter, const Vector2d &v1, const Vector2d &v2, const Vector2d &v3, const float lineWidth, const QVector<QColor> triangleColors) const
{
    if (triangleColors.length() != 3)
        return;
    QVector<QVector2D> planeC = { translateVec2(v1), translateVec2(v2), translateVec2(v3) };
    drawTriDotTriangle(painter, planeC, lineWidth, triangleColors);
}

void EsDrawer::drawTriangleUVmonocolor(QPainter *painter, const Vector2d &v1, const Vector2d &v2, const Vector2d &v3, const float lineWidth, const QColor triangleColor) const
{
    drawTriangleUV(painter, v1,v2,v3, lineWidth, {triangleColor, triangleColor, triangleColor});
}

void EsDrawer::debugTriangle(QPainter *painter, const Eigen::Matrix3d &mat, const QColor &fillColor, const float width) const
{
    drawTriangleGmonocolor(painter, mat.col(0), mat.col(1), mat.col(2), width, fillColor);
}

void EsDrawer::debugTriangleUV(QPainter *painter, const Matrix3d &mat, const QColor &fillColor, const float width) const
{
    const MatrixXd bl = mat.block<2, 3>(0,0);
    drawTriangleUVmonocolor(painter, bl.col(0), bl.col(1), bl.col(2), width, fillColor);
}

//void EsDrawer::debugTriangle(QPainter *painter, const Mat23D &mat) const
//{
//    drawTriangleG(painter,
//               makeVector3d(mat(0,0), mat(1,0), .0),
//               makeVector3d(mat(0,1), mat(1,1), .0),
//               makeVector3d(mat(0,2), mat(1,2), .0));
//}

QVector2D EsDrawer::translateVec3(const Vector3d &v) const
{
    return QVector2D(int(m_scaling * (v(0,0) + m_zToXProect * v(2,0))) + m_centerG.x(),
                     int(m_scaling * (- v(1,0) + m_zToYProect * v(2,0))) + m_centerG.y());
}

QVector2D EsDrawer::translateVec2(const Vector2d &v, const bool isUV) const
{
    if (isUV)
        return QVector2D(int(m_sizeUV * v(0,0)) + m_centerUV.x(),
                         int(m_sizeUV * v(1,0)) + m_centerUV.y());

    return QVector2D(int(m_scaling * v(0,0)) + m_centerG.x(),
                     int(m_scaling * v(1,0)) + m_centerG.y());
}

int colorSwap = 0;
void EsDrawer::drawTriDotTriangle(QPainter *painter, const QVector<QVector2D> &dots, const float lineWidth, QVector<QColor> triangleColors) const
{
    if (triangleColors.length() != 3 || dots.length() != 3)
        return;
    QPolygon p;
    p << QPoint(dots[0].x(), dots[0].y())
        << QPoint(dots[1].x(), dots[1].y())
        << QPoint(dots[2].x(), dots[2].y());
    const int cl = colorSwap % 3;
    ++colorSwap;
    //painter->setBrush(QColor(triangleColors[cl].red(),triangleColors[cl].green(), triangleColors[cl].blue(), 125));
    painter->drawPolygon(p);
    return;
    for (int from = 0; from < 3; ++from){
        const int to = (from + 1) % 3;
        painter->setPen(QPen(triangleColors[from], lineWidth));
        painter->drawLine(dots[from].x(), dots[from].y(), dots[to].x(), dots[to].y());
    }
}
