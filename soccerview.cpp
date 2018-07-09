//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
\file    soccerview.cpp
\brief   C++ Implementation: GLSoccerView
\author  Joydeep Biswas (C) 2011
*/
//========================================================================

#include "soccerview.h"

#include "field.h"
#include "field_default_constants.h"

using FieldConstants::kNumFieldLines;
using FieldConstants::kNumFieldArcs;
using FieldConstants::kNumFieldTriangles;
using FieldConstants::kFieldLines;
using FieldConstants::kFieldArcs;
using FieldConstants::kFieldTris;

const double GLSoccerView::minZValue = -10;
const double GLSoccerView::maxZValue = 10;
const double GLSoccerView::FieldZ  = 1.0;
const double GLSoccerView::RobotZ  = 2.0;
const double GLSoccerView::BallZ   = 3.0;
const double GLSoccerView::DebugZ  = 4.0;
const int GLSoccerView::PreferedWidth = 1024;
const int GLSoccerView::PreferedHeight = 768;
const double GLSoccerView::MinRedrawInterval = 0.016; ///Minimum time between graphics updates (limits the fps)
const int GLSoccerView::unknownRobotID = -1;

GLSoccerView::FieldDimensions::FieldDimensions() :
    field_length(FieldConstants::kFieldLength),
    field_width(FieldConstants::kFieldWidth),
    boundary_width(FieldConstants::kBoundaryWidth) {
    for (size_t i = 0; i < kNumFieldLines; ++i) {
        lines.push_back(new FieldLine(kFieldLines[i]));
    }
    for (size_t i = 0; i < kNumFieldArcs; ++i) {
        arcs.push_back(new FieldCircularArc(kFieldArcs[i]));
    }
    for (size_t i = 0; i < kNumFieldTriangles; ++i) {
        tris.push_back(new FieldTriangle(kFieldTris[i]));
    }
}

GLSoccerView::GLSoccerView(QWidget* parent) :
    QGLWidget(QGLFormat(
                  QGL::DoubleBuffer | QGL::DepthBuffer | QGL::SampleBuffers),parent) {
    viewScale =
            (fieldDim.field_length + fieldDim.boundary_width) / sizeHint().width();
    viewScale = max(viewScale,
                    (fieldDim.field_width + fieldDim.boundary_width) /
                    sizeHint().height());

    viewXOffset = viewYOffset = 0.0;
    setAutoFillBackground(false); //Do not let painter auto fill the widget's background: we'll do it manually through openGl
    connect(this, SIGNAL(postRedraw()), this, SLOT(redraw()));
    blueRobotShape = GL_INVALID_VALUE;
    yellowRobotShape = GL_INVALID_VALUE;
    greyRobotShape = GL_INVALID_VALUE;
    blueCircleRobotShape = GL_INVALID_VALUE;
    yellowCircleRobotShape = GL_INVALID_VALUE;
    greyCircleRobotShape = GL_INVALID_VALUE;
    QFont RobotIDFont = this->font();
    RobotIDFont.setWeight(QFont::Bold);
    RobotIDFont.setPointSize(80);
    glText = GLText(RobotIDFont);
    tLastRedraw = 0;
}

void GLSoccerView::updatePacket(const DataWrapper &_packet) {

    graphicsMutex.lock();
    ball.x = ball.y = 5000;
    robots.clear();
    bool showRaw = true;

    if (showRaw) {
        if (_packet.has_detection()) {
            updateDetection(_packet.detection());
        }
    } else {
        if (_packet.has_worldmodel()) {
            updateWorldModel(_packet.worldmodel());
        }
    }
    if (_packet.has_draws()) {
        debugs = _packet.draws();

    }
    graphicsMutex.unlock();
    postRedraw();
}

void GLSoccerView::updateDetection(const Frame &_frame) {

    for(int i=0; i < _frame.robots_blue_size(); i++){
        Robot robot;
        robot.loc.set(_frame.robots_blue(i).x(), _frame.robots_blue(i).y());
        robot.id = i;
        robot.hasAngle = true;
        if(robot.hasAngle) robot.angle = _frame.robots_blue(i).ang();
        robot.team = teamBlue;
        robot.conf = 0.5;
        robots.append(robot);
    }

    for(int i=0; i < _frame.robots_yellow_size(); i++){
        Robot robot;
        robot.loc.set(_frame.robots_yellow(i).x(), _frame.robots_yellow(i).y());
        robot.id = i;
        robot.hasAngle = true;
        if(robot.hasAngle) robot.angle = _frame.robots_yellow(i).ang();
        robot.team = teamYellow;
        robot.conf = 0.5;
        robots.append(robot);
    }

    ball.x = _frame.ball().x();
    ball.y = _frame.ball().y();
}

void GLSoccerView::updateWorldModel(const WorldModel &_wm) {

    for(int i=0; i < _wm.robots_blue_size(); i++){
        Robot robot;
        robot.loc.set(_wm.robots_blue(i).pos().x(), _wm.robots_blue(i).pos().y());
        robot.id = i;
        robot.hasAngle = true;
        if(robot.hasAngle) robot.angle = _wm.robots_blue(i).direction();
        robot.team = teamBlue;
        robot.conf = 0.5;
        robots.append(robot);
    }

    for(int i=0; i < _wm.robots_yellow_size(); i++){
        Robot robot;
        robot.loc.set(_wm.robots_yellow(i).pos().x(), _wm.robots_yellow(i).pos().y());
        robot.id = i;
        robot.hasAngle = true;
        if(robot.hasAngle) robot.angle = _wm.robots_yellow(i).direction();
        robot.team = teamYellow;
        robot.conf = 0.5;
        robots.append(robot);
    }

    ball.x = _wm.ball().pos().x();
    ball.y = _wm.ball().pos().y();
}

void GLSoccerView::redraw()
{
    if(GetTimeSec()-tLastRedraw<MinRedrawInterval)
        return;
    graphicsMutex.lock();
    update();
    graphicsMutex.unlock();
    tLastRedraw = GetTimeSec();
}


void GLSoccerView::mousePressEvent(QMouseEvent* event)
{
    leftButton = event->buttons().testFlag(Qt::LeftButton);
    midButton = event->buttons().testFlag(Qt::MidButton);
    rightButton = event->buttons().testFlag(Qt::RightButton);

    if(leftButton)
        setCursor(Qt::ClosedHandCursor);
    if(midButton)
        setCursor(Qt::SizeVerCursor);
    if(leftButton || midButton){
        // Start Pan / Zoom
        mouseStartX = event->x();
        mouseStartY = event->y();
        postRedraw();
    }
}

void GLSoccerView::mouseReleaseEvent(QMouseEvent* event)
{
    setCursor(Qt::ArrowCursor);
}

void GLSoccerView::mouseMoveEvent(QMouseEvent* event)
{
    static const bool debug = false;
    bool leftButton = event->buttons().testFlag(Qt::LeftButton);
    bool midButton = event->buttons().testFlag(Qt::MidButton);
    bool rightButton = event->buttons().testFlag(Qt::RightButton);

    if(debug) printf("MouseMove Event, Left:%d Mid:%d Right:%d\n", leftButton?1:0, midButton?1:0, rightButton?1:0);

    if(leftButton){
        //Pan
        viewXOffset -= viewScale*double(event->x() - mouseStartX);
        viewYOffset += viewScale*double(event->y() - mouseStartY);
        mouseStartX = event->x();
        mouseStartY = event->y();
        recomputeProjection();
        postRedraw();
    }else if(midButton){
        //Zoom
        double zoomRatio = double(event->y() - mouseStartY)/500.0;
        viewScale = viewScale*(1.0+zoomRatio);
        recomputeProjection();
        mouseStartX = event->x();
        mouseStartY = event->y();
        postRedraw();
    }
}

void GLSoccerView::wheelEvent(QWheelEvent* event)
{
    static const bool debug = false;
    double zoomRatio = -double(event->delta())/1000.0;
    viewScale = viewScale*(1.0+zoomRatio);
    recomputeProjection();
    if(debug) printf("Zoom: %5.3f\n",viewScale);
    postRedraw();
}

void GLSoccerView::keyPressEvent(QKeyEvent* event)
{
    static const bool debug = false;
    if(debug) printf("KeyPress: 0x%08X\n",event->key());
    if(event->key() == Qt::Key_Space)
        resetView();
    if(event->key() == Qt::Key_Escape)
        close();
}

void GLSoccerView::resetView()
{
    viewScale =
            (fieldDim.field_length + fieldDim.boundary_width) / width();
    viewScale = max(viewScale,
                    (fieldDim.field_width + fieldDim.boundary_width) / height());

    viewXOffset = viewYOffset = 0.0;
    recomputeProjection();
    postRedraw();
}

void GLSoccerView::resizeEvent(QResizeEvent* event)
{
    QGLWidget::resizeEvent(event);
    redraw();
}

void GLSoccerView::recomputeProjection()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-0.5*viewScale*width()+viewXOffset, 0.5*viewScale*width()+viewXOffset, -0.5*viewScale*height()+viewYOffset, 0.5*viewScale*height()+viewYOffset, minZValue, maxZValue);
    glMatrixMode(GL_MODELVIEW);
}

void GLSoccerView::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    recomputeProjection();
}

void GLSoccerView::initializeGL()
{
    blueRobotShape = glGenLists(1);
    if(blueRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(blueRobotShape, GL_COMPILE);
    drawRobot(teamBlue,true,false);
    glEndList();

    yellowRobotShape = glGenLists(1);
    if(yellowRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(yellowRobotShape, GL_COMPILE);
    drawRobot(teamYellow,true,false);
    glEndList();

    greyRobotShape = glGenLists(1);
    if(greyRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(greyRobotShape, GL_COMPILE);
    drawRobot(teamUnknown,true,false);
    glEndList();

    blueCircleRobotShape = glGenLists(1);
    if(blueRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(blueCircleRobotShape, GL_COMPILE);
    drawRobot(teamBlue,false,false);
    glEndList();

    yellowCircleRobotShape = glGenLists(1);
    if(yellowRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(yellowCircleRobotShape, GL_COMPILE);
    drawRobot(teamYellow,false,false);
    glEndList();

    greyCircleRobotShape = glGenLists(1);
    if(greyRobotShape==GL_INVALID_VALUE){
        printf("Unable to create display list!\n");
        exit(1);
    }
    glNewList(greyCircleRobotShape, GL_COMPILE);
    drawRobot(teamUnknown,false,false);
    glEndList();
}


void GLSoccerView::vectorTextTest()
{
#define TextTest(loc,angle,size,str,halign,valign) \
    {glText.drawString((loc),angle,size,str,halign,valign); \
    vector2d l1,l2; \
    l1.heading(M_PI/180.0*angle); \
    l1 = loc+l1*size*(glText.getWidth(str)); \
    l2.heading(M_PI/180.0*angle); \
    l2 = loc-l2*size*(glText.getWidth(str)); \
    glBegin(GL_LINES); \
    glVertex3d(l1.x,l1.y,9); \
    glVertex3d(l2.x,l2.y,9); \
    glEnd();}

    glColor3d(1,1,1);
    TextTest(vector2d(1,1)*353.6,45,500,"123agdo0",GLText::LeftAligned,GLText::MedianAligned)
            TextTest(vector2d(fieldDim.field_length*0.5,0),0,500,"123agdo0",GLText::RightAligned,GLText::BottomAligned)
            TextTest(vector2d(0,-fieldDim.field_width*0.5),0,500,"123agdo0",GLText::CenterAligned,GLText::TopAligned)
            TextTest(vector2d(-fieldDim.field_length*0.5,0),0,500,"1\ub023agdo0",GLText::CenterAligned,GLText::MiddleAligned)
}

void GLSoccerView::paintEvent(QPaintEvent* event)
{
    graphicsMutex.lock();
    makeCurrent();
    glClearColor(FIELD_COLOR);
    glShadeModel(GL_SMOOTH);
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    drawFieldLines(fieldDim);
    drawRobots();
    drawBall(ball);
    drawDebugs();
//    vectorTextTest();
    glPopMatrix();
    swapBuffers();
    graphicsMutex.unlock();
}

void GLSoccerView::drawQuad(vector2d loc1, vector2d loc2, double z, bool fill)
{
    if (fill) glBegin(GL_QUADS);
    else glBegin(GL_LINE_LOOP);
    glVertex3d(loc1.x,loc1.y,z);
    glVertex3d(loc2.x,loc1.y,z);
    glVertex3d(loc2.x,loc2.y,z);
    glVertex3d(loc1.x,loc2.y,z);
    glEnd();
}

void GLSoccerView::drawArc(vector2d loc, double r1, double r2, double theta1, double theta2, double z, double dTheta, bool fill)
{
    static const double tesselation = 1.0;
    if(dTheta<0){
        dTheta = tesselation/r2;
    }
    if (fill) r1 = 0;
    glBegin(GL_QUAD_STRIP);
    for(double theta=theta1; theta<theta2; theta+=dTheta){
        double c1 = cos(theta), s1 = sin(theta);
        glVertex3d(r2*c1+loc.x,r2*s1+loc.y,z);
        glVertex3d(r1*c1+loc.x,r1*s1+loc.y,z);
    }
    double c1 = cos(theta2), s1 = sin(theta2);
    glVertex3d(r2*c1+loc.x,r2*s1+loc.y,z);
    glVertex3d(r1*c1+loc.x,r1*s1+loc.y,z);
    glEnd();
}

void GLSoccerView::drawPoint(double x, double y, double z) {
    drawLine(x - 1, y + 1,
             x + 1, y - 1,
             z);
    drawLine(x + 1, y + 1,
             x - 1, y - 1,
             z);
}

void GLSoccerView::drawLine(double x1, double y1, double x2, double y2, double z) {
    glBegin(GL_LINES);
    glVertex3d(x1, y1, z);
    glVertex3d(x2, y2, z);
    glEnd();
}

void GLSoccerView::drawRobot(int team, bool hasAngle, bool useDisplayLists)
{
    if(useDisplayLists){
        switch ( team ){
        case teamBlue:{
            if(hasAngle)
                glCallList(blueRobotShape);
            else
                glCallList(blueCircleRobotShape);
            break;
        }
        case teamYellow:{
            if(hasAngle)
                glCallList(yellowRobotShape);
            else
                glCallList(yellowCircleRobotShape);
            break;
        }
        default:{
            if(hasAngle)
                glCallList(greyRobotShape);
            else
                glCallList(greyCircleRobotShape);
            break;
        }
        }
        return;
    }

    switch ( team ){
    case teamBlue:{
        glColor3d(0.2549, 0.4941, 1.0);
        break;
    }
    case teamYellow:{
        glColor3d(1.0, 0.9529, 0.2431);
        break;
    }
    default:{
        glColor3d(0.5882,0.5882,0.5882);
        break;
    }
    }
    drawQuad(-5,5,5,-5,RobotZ);

    switch ( team ){
    case teamBlue:{
        glColor3d(0.0706, 0.2314, 0.6275);
        break;
    }
    case teamYellow:{
        glColor3d(0.8, 0.6157, 0.0);
        break;
    }
    default:{
        glColor3d(0.2745,0.2745,0.2745);
        break;
    }
    }
    drawQuad(-5,5,-4.5,-5,RobotZ+0.01);
    drawQuad(-5,5,5,4.5,RobotZ+0.01);
    drawQuad(-5,-4.5,5,-5,RobotZ+0.01);

    glColor3d(1,0,0);
    drawQuad(4,5,5,-5,RobotZ+0.01);


    if(hasAngle) {
        double theta1 = hasAngle?RAD(40):0.0;
        double theta2 = 2.0*M_PI - theta1;
        glColor3d(0.2745,0.2745,0.2745);
        drawQuad(-5.*cos(theta1),5.*sin(theta1), 5.*cos(theta2), -5.*sin(theta2) + 1,RobotZ+0.02);
        drawQuad(-5.*cos(theta2),-5.*sin(theta1), 5.*cos(theta1), 5.*sin(theta2) - 1 ,RobotZ+0.02);

    }
}

void GLSoccerView::drawRobot(vector2d loc, double theta, double conf, int robotID, int team, bool hasAngle)
{
    glPushMatrix();
    glLoadIdentity();
    glTranslated(loc.x,loc.y,0);
    switch ( team ){
    case teamBlue:{
        glColor3d(0.2549, 0.4941, 1.0);
        break;
    }
    case teamYellow:{
        glColor3d(1.0, 0.9529, 0.2431);
        break;
    }
    default:{
        glColor3d(0.5882,0.5882,0.5882);
        break;
    }
    }
    drawQuad(-9,13,-9+18*conf,16,RobotZ);
    glColor3d(0.0,0.0,0.0);
    char buf[1024];
    if(robotID!=unknownRobotID)
        snprintf(buf,1023,"%X",robotID);
    else
        snprintf(buf,1023,"?");
    glText.drawString(loc,0,5,buf,GLText::CenterAligned,GLText::MiddleAligned);
    switch ( team ){
    case teamBlue:{
        glColor3d(0.0706, 0.2314, 0.6275);
        break;
    }
    case teamYellow:{
        glColor3d(0.8, 0.6157, 0.0);
        break;
    }
    default:{
        glColor3d(0.2745,0.2745,0.2745);
        break;
    }
    }
    drawQuad(-9.6,12.4,9.6,13.0,RobotZ+0.01);
    drawQuad(-9.6,12.4,-9.0,16.6,RobotZ+0.01);
    drawQuad(-9.6,16.0,9.6,16.6,RobotZ+0.01);
    drawQuad(9.0,12.4,9.6,16.6,RobotZ+0.01);

    glRotated(theta,0,0,1.0);
    drawRobot(team, hasAngle, true);
    glPopMatrix();
}

void GLSoccerView::drawFieldLines(FieldDimensions& dimensions)
{
    glColor4f(FIELD_LINES_COLOR);
    for (size_t i = 0; i < fieldDim.lines.size(); ++i) {
        const FieldLine& line = *fieldDim.lines[i];
        const double half_thickness = 0.5 * line.thickness;
        const vector2d p1(line.p1_x, line.p1_y);
        const vector2d p2(line.p2_x, line.p2_y);
        const vector2d perp = (p2 - p1).norm().perp();
        const vector2d corner1 = p1 - half_thickness * perp;
        const vector2d corner2 = p2 + half_thickness * perp;
        drawQuad(corner1, corner2, FieldZ);
    }

    for (size_t i = 0; i < fieldDim.arcs.size(); ++i) {
        const FieldCircularArc& arc = *fieldDim.arcs[i];
        const double half_thickness = 0.5 * arc.thickness;
        const double radius = arc.radius;
        const vector2d center(arc.center_x, arc.center_y);
        const double a1 = arc.a1;
        const double a2 = arc.a2;
        drawArc(center, radius - half_thickness, radius + half_thickness, a1, a2,
                FieldZ);
    }

    for (size_t i = 0; i < fieldDim.tris.size(); ++i) {
        const FieldTriangle& triangle = *fieldDim.tris[i];
        vector2d a,b,c;
        a.x = triangle.p1_x;
        a.y = triangle.p1_y;
        b.x = triangle.p2_x;
        b.y = triangle.p2_y;
        c.x = triangle.p3_x;
        c.y = triangle.p3_y;
        drawTriangle(a, b, c, FieldZ);
    }
    vector2d a;
    a.x = a.y = 0;
    drawRobot(a, 10, 0.9, 3, 1, true);
}

void GLSoccerView::drawBall(vector2d loc)
{
    glColor3d(1.0,0.5059,0.0);
    drawArc(loc,0,1.6,-M_PI,M_PI,BallZ);
    glColor3d(0.8706,0.3490,0.0);
    drawArc(loc,1.5,2.1,-M_PI,M_PI,BallZ);

}

void GLSoccerView::drawTriangle(vector2d loc1, vector2d loc2, vector2d loc3, double z) {
    glBegin(GL_TRIANGLES);
    glVertex3d(loc1.x,loc1.y,z);
    glVertex3d(loc2.x,loc2.y,z);
    glVertex3d(loc3.x,loc3.y,z);
    glEnd();
}

void GLSoccerView::drawRobots() {
    for(int i=0; i<robots.size(); i++){
        Robot r = robots[i];
        drawRobot(r.loc,r.angle,r.conf,r.id,r.team,r.hasAngle);
    }
}

void GLSoccerView::drawDebugs() {
    for (int i = 0; i < debugs.vectors_size(); i++) {
        const Vec2D& v = debugs.vectors(i);
        glColor4f(v.color().r(),
                  v.color().g(),
                  v.color().b(),
                  v.color().a());
        drawPoint(v.x(), v.y(), DebugZ);
    }

    for (int i = 0; i < debugs.circles_size(); i++) {
        const Cir2D& c = debugs.circles(i);
        glColor4f(c.color().r(),
                  c.color().g(),
                  c.color().b(),
                  c.color().a());
        drawArc(c.center().x(), c.center().y(),
                (c.fill()) ? 0 : c.radius() - 0.1, c.radius(),
                c.startangle(), c.endangle(), DebugZ, -1, c.fill());
    }

    for (int i = 0; i < debugs.rects_size(); i++) {
        const Rec2D& r = debugs.rects(i);
        glColor4f(r.color().r(),
                  r.color().g(),
                  r.color().b(),
                  r.color().a());
        drawQuad(r.topleft().x(), r.topleft().y(),
                 r.botright().x(), r.botright().y(), DebugZ, r.fill());
    }

    for (int i = 0; i < debugs.segments_size(); i++) {
        const Seg2D& s = debugs.segments(i);
        glColor4f(s.color().r(),
                  s.color().g(),
                  s.color().b(),
                  s.color().a());
        drawLine(s.origin().x(), s.origin().y(),
                 s.terminal().x(), s.terminal().y(), DebugZ);

    }

    for (int i = 0; i < debugs.texts_size(); i++) {
        const Tex2D& t = debugs.texts(i);
        QString q(t.data().c_str());
        q.replace(' ', '_');
        glColor4f(t.color().r(),
                  t.color().g(),
                  t.color().b(),
                  t.color().a());
        glText.drawString(t.pos().x(), t.pos().y(), 0, t.size(), q.toStdString().c_str(),GLText::CenterAligned,GLText::MiddleAligned);

    }

    for (int i = 0; i < debugs.polygons_size(); i++) {
        const Pol2D& p = debugs.polygons(i);
        glColor4f(p.color().r(),
                  p.color().g(),
                  p.color().b(),
                  p.color().a());
        if (p.fill()) glBegin(GL_TRIANGLE_FAN);
        else glBegin(GL_LINE_LOOP);
        for (int j = 0; j < p.vectors_size(); j++) {
            const Vec2& v = p.vectors(j);
            glVertex3d(v.x(), v.y(), DebugZ);
        }
        glEnd();

    }
}
