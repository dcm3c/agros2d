// This file is part of Agros2D.
//
// Agros2D is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// Agros2D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Agros2D.  If not, see <http://www.gnu.org/licenses/>.
//
// hp-FEM group (http://hpfem.org/)
// University of Nevada, Reno (UNR) and University of West Bohemia, Pilsen
// Email: agros2d@googlegroups.com, home page: http://hpfem.org/agros2d/

#ifndef PROBLEM_CONFIG_H
#define PROBLEM_CONFIG_H

#include "util.h"
#include "value.h"
#include "solutiontypes.h"

class FieldInfo;
class CouplingInfo;

class Field;
class Problem;

class ProblemConfig : public QObject
{
    Q_OBJECT
public:
    ProblemConfig(QWidget *parent = 0);

    inline QString labelX() { return ((m_coordinateType == CoordinateType_Planar) ? "X" : "R");  }
    inline QString labelY() { return ((m_coordinateType == CoordinateType_Planar) ? "Y" : "Z");  }
    inline QString labelZ() { return ((m_coordinateType == CoordinateType_Planar) ? "Z" : "a");  }

    void clear();

    inline QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    inline QString fileName() const { return m_fileName; }
    void setFileName(const QString &fileName) { m_fileName = fileName; }

    // coordinates
    inline CoordinateType coordinateType() const { return m_coordinateType; }
    void setCoordinateType(const CoordinateType coordinateType) { m_coordinateType = coordinateType; emit changed(); }

    // harmonic problem
    inline double frequency() const { return m_frequency; }
    void setFrequency(const double frequency) { m_frequency = frequency; emit changed(); }

    // transient problem
    inline int numConstantTimeSteps() const { return m_numConstantTimeSteps; }
    void setNumConstantTimeSteps(const int numConstantTimeSteps) { m_numConstantTimeSteps = numConstantTimeSteps; emit changed(); }

    inline Value timeTotal() const { return m_timeTotal; }
    void setTimeTotal(const Value &timeTotal) { m_timeTotal = timeTotal; emit changed(); }

    inline double constantTimeStepLength() { return m_timeTotal.number() / m_numConstantTimeSteps; }
    double initialTimeStepLength();

    inline TimeStepMethod timeStepMethod() const {return m_timeStepMethod; }
    void setTimeStepMethod(TimeStepMethod timeStepMethod) { m_timeStepMethod = timeStepMethod; }
    bool isTransientAdaptive() const;

    int timeOrder() const { return m_timeOrder; }
    void setTimeOrder(int timeOrder) {m_timeOrder = timeOrder; }

    inline Value timeMethodTolerance() const { return m_timeMethodTolerance; }
    void setTimeMethodTolerance(Value timeMethodTolerance) {m_timeMethodTolerance = timeMethodTolerance; }

    // matrix
    inline Hermes::MatrixSolverType matrixSolver() const { return m_matrixSolver; }
    void setMatrixSolver(const Hermes::MatrixSolverType matrixSolver) { m_matrixSolver = matrixSolver; emit changed(); }

    // mesh
    inline MeshType meshType() const { return m_meshType; }
    void setMeshType(const MeshType meshType) { m_meshType = meshType; emit changed(); }

    // startup script
    inline QString startupscript() const { return m_startupscript; }
    void setStartupScript(const QString &startupscript) { m_startupscript = startupscript; emit changed(); }

    // description
    inline QString description() const { return m_description; }
    void setDescription(const QString &description) { m_description = description; }

    void refresh() { emit changed(); }

signals:
    void changed();

private:
    QString m_name;
    QString m_fileName;
    CoordinateType m_coordinateType;

    // harmonic
    double m_frequency;

    // transient
    Value m_timeTotal;
    int m_numConstantTimeSteps;
    TimeStepMethod m_timeStepMethod;
    int m_timeOrder;
    Value m_timeMethodTolerance;

    // matrix solver
    Hermes::MatrixSolverType m_matrixSolver;

    // mesh type
    MeshType m_meshType;

    QString m_startupscript;
    QString m_description;
};

class ProblemConfigView : public QObject
{
    Q_OBJECT

public:
    ProblemConfigView();
    ~ProblemConfigView();

    // font
    QString rulersFont;
    QString postFont;

    // zoom
    bool zoomToMouse;

    // geometry
    double nodeSize;
    double edgeWidth;
    double labelSize;

    // rulers
    bool showRulers;

    // colors
    QColor colorBackground;
    QColor colorGrid;
    QColor colorCross;
    QColor colorNodes;
    QColor colorEdges;
    QColor colorLabels;
    QColor colorContours;
    QColor colorVectors;
    QColor colorInitialMesh;
    QColor colorSolutionMesh;
    QColor colorHighlighted;
    QColor colorCrossed;
    QColor colorSelected;
    QColor colorNotConnected;

    // grid
    bool showGrid;
    double gridStep;
    bool snapToGrid;

    // axes
    bool showAxes;

    // deformations
    bool deformScalar;
    bool deformContour;
    bool deformVector;

    // 3d
    bool scalarView3DLighting;
    double scalarView3DAngle;
    bool scalarView3DBackground;
    double scalarView3DHeight;
    bool scalarView3DBoundingBox;

    // active field
    QString activeField;

    // post3d
    SceneViewPost3DMode showPost3D;

    // linearizer quality
    PaletteQuality linearizerQuality;

    // contour
    bool showContourView;
    QString contourVariable;
    int contoursCount;
    double contourWidth;

    // scalar view
    bool showScalarView;
    QString scalarVariable;
    PhysicFieldVariableComp scalarVariableComp;
    bool scalarRangeAuto;
    double scalarRangeMin;
    double scalarRangeMax;
    bool scalarRangeLog;
    double scalarRangeBase;
    int scalarDecimalPlace;

    // vector view
    bool showVectorView;
    QString vectorVariable;
    bool vectorProportional;
    bool vectorColor;
    int vectorCount;
    double vectorScale;
    VectorType vectorType;
    VectorCenter vectorCenter;

    // mesh
    bool showInitialMeshView;
    bool showSolutionMeshView;
    int angleSegmentsCount;
    bool curvilinearElements;

    // order view
    bool showOrderView;
    bool showOrderColorBar;
    PaletteOrderType orderPaletteOrderType;
    bool orderLabel;

    // palette
    bool showScalarColorBar;
    PaletteType paletteType;
    int paletteSteps;
    bool paletteFilter;

    // particle tracing
    bool showParticleView;
    bool particleIncludeRelativisticCorrection;
    double particleMass;
    double particleConstant;
    double particleMaximumRelativeError;
    double particleMinimumStep;
    int particleMaximumNumberOfSteps;
    bool particleReflectOnDifferentMaterial;
    bool particleReflectOnBoundary;
    double particleCoefficientOfRestitution;
    Point particleStart;
    Point particleStartVelocity;
    Point particleAcceleration;
    Point3 particleCustomForce;
    int particleNumberOfParticles;
    double particleStartingRadius;
    bool particleColorByVelocity;
    bool particleShowPoints;
    double particleDragDensity;
    double particleDragCoefficient;
    double particleDragReferenceArea;

    // solid view
    QStringList solidViewHide;

    // adaptivity
    int maxDofs;
    bool isoOnly;
    double convExp;
    double threshold;
    int strategy;
    int meshRegularity;
    Hermes::Hermes2D::ProjNormType projNormType;
    bool useAniso;
    bool finerReference;

    // command argument
    QString commandTriangle;
    QString commandGmsh;

    void load(QDomElement *config);
    void save(QDomElement *config);

    void clear();

private:
    QDomElement *eleConfig;

    bool readConfig(const QString &key, bool defaultValue);
    int readConfig(const QString &key, int defaultValue);
    double readConfig(const QString &key, double defaultValue);
    QString readConfig(const QString &key, const QString &defaultValue);
    QColor readConfig(const QString &key, const QColor &defaultValue);
    QStringList readConfig(const QString &key, const QStringList &defaultValue);

    void writeConfig(const QString &key, bool value);
    void writeConfig(const QString &key, int value);
    void writeConfig(const QString &key, double value);
    void writeConfig(const QString &key, const QString &value);
    void writeConfig(const QString &key, const QColor &value);
    void writeConfig(const QString &key, const QStringList &value);
};

#endif // PROBLEM_CONFIG_H