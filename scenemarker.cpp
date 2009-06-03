#include "scenemarker.h"

SceneEdgeMarker::SceneEdgeMarker(const QString &name, PhysicFieldBC type)
{
    this->name = name;
    this->type = type;
}

QVariant SceneEdgeMarker::variant()
{
    QVariant v;
    v.setValue(this);
    return v;
}

SceneEdgeMarkerNone::SceneEdgeMarkerNone() : SceneEdgeMarker("none", PHYSICFIELDBC_NONE)
{
}

// *************************************************************************************************************************************

SceneEdgeElectrostaticMarker::SceneEdgeElectrostaticMarker(const QString &name, PhysicFieldBC type, double value)
    : SceneEdgeMarker(name, type)
{
    this->value = value;
}

int SceneEdgeElectrostaticMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneEdgeElectrostaticMarker *dialog = new DSceneEdgeElectrostaticMarker(this, parent);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneEdgeMagnetostaticMarker::SceneEdgeMagnetostaticMarker(const QString &name, PhysicFieldBC type, double value)
    : SceneEdgeMarker(name, type)
{
    this->value = value;
}

int SceneEdgeMagnetostaticMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneEdgeMagnetostaticMarker *dialog = new DSceneEdgeMagnetostaticMarker(this, parent);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneEdgeHeatMarker::SceneEdgeHeatMarker(const QString &name, PhysicFieldBC type, double temperature) : SceneEdgeMarker(name, type)
{
    this->temperature = temperature;
}

SceneEdgeHeatMarker::SceneEdgeHeatMarker(const QString &name, PhysicFieldBC type, double heatFlux, double h, double externalTemperature) : SceneEdgeMarker(name, type)
{
    this->heatFlux = heatFlux;
    this->h = h;
    this->externalTemperature = externalTemperature;
}

int SceneEdgeHeatMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneEdgeHeatMarker *dialog = new DSceneEdgeHeatMarker(this, parent);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneEdgeElasticityMarker::SceneEdgeElasticityMarker(const QString &name, PhysicFieldBC typeX, PhysicFieldBC typeY, double forceX, double forceY)
    : SceneEdgeMarker(name, typeX)
{
    this->typeX = typeX;
    this->typeY = typeY;
    this->forceX = forceX;
    this->forceY = forceY;
}

int SceneEdgeElasticityMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneEdgeElasticityMarker *dialog = new DSceneEdgeElasticityMarker(this, parent);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneLabelMarker::SceneLabelMarker(const QString &name)
{
    this->name = name;
}

QVariant SceneLabelMarker::variant()
{
    QVariant v;
    v.setValue(this);
    return v;
}

SceneLabelMarkerNone::SceneLabelMarkerNone() : SceneLabelMarker("none")
{
}

// *************************************************************************************************************************************

SceneLabelElectrostaticMarker::SceneLabelElectrostaticMarker(const QString &name, double charge_density, double permittivity)
    : SceneLabelMarker(name)
{
    this->charge_density = charge_density;
    this->permittivity = permittivity;
}

int SceneLabelElectrostaticMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneLabelElectrostaticMarker *dialog = new DSceneLabelElectrostaticMarker(parent, this);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneLabelHeatMarker::SceneLabelHeatMarker(const QString &name, double volume_heat, double thermal_conductivity)
    : SceneLabelMarker(name)
{
    this->thermal_conductivity = thermal_conductivity;
    this->volume_heat = volume_heat;
}

int SceneLabelHeatMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneLabelHeatMarker *dialog = new DSceneLabelHeatMarker(parent, this);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneLabelMagnetostaticMarker::SceneLabelMagnetostaticMarker(const QString &name, double current_density, double permeability)
    : SceneLabelMarker(name)
{
    this->permeability = permeability;
    this->current_density = current_density;
}

int SceneLabelMagnetostaticMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneLabelMagnetostaticMarker *dialog = new DSceneLabelMagnetostaticMarker(parent, this);
    return dialog->exec();
}

// *************************************************************************************************************************************

SceneLabelElasticityMarker::SceneLabelElasticityMarker(const QString &name, double young_modulus, double poisson_ratio)
    : SceneLabelMarker(name)
{
    this->young_modulus = young_modulus;
    this->poisson_ratio = poisson_ratio;
}

int SceneLabelElasticityMarker::showDialog(Scene *scene, QWidget *parent)
{
    DSceneLabelElasticityMarker *dialog = new DSceneLabelElasticityMarker(parent, this);
    return dialog->exec();
}

// *************************************************************************************************************************************

DSceneEdgeMarker::DSceneEdgeMarker(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout();
    txtName = new QLineEdit("");
}

DSceneEdgeMarker::~DSceneEdgeMarker()
{
    delete layout;
    delete txtName;
}

void DSceneEdgeMarker::createDialog()
{
    QHBoxLayout *layoutName = new QHBoxLayout();
    layoutName->addWidget(new QLabel(tr("Name:")));
    layoutName->addWidget(txtName);

    // dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(doReject()));

    layout->addLayout(layoutName);
    layout->addLayout(createContent());
    layout->addStretch();
    layout->addWidget(buttonBox);

    setLayout(layout);
}

void DSceneEdgeMarker::load()
{
    txtName->setText(m_edgeMarker->name);
}

void DSceneEdgeMarker::save()
{
    m_edgeMarker->name = txtName->text();
}

void DSceneEdgeMarker::doAccept()
{
    save();

    accept();
}

void DSceneEdgeMarker::doReject()
{
    reject();
}

// *************************************************************************************************************************************

DSceneEdgeElectrostaticMarker::DSceneEdgeElectrostaticMarker(SceneEdgeElectrostaticMarker *edgeElectrostaticMarker, QWidget *parent) : DSceneEdgeMarker(parent)
{
    this->m_edgeMarker = edgeElectrostaticMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Boundary Condition"));

    createDialog();

    load();
}

DSceneEdgeElectrostaticMarker::~DSceneEdgeElectrostaticMarker()
{
    delete cmbType;
    delete txtValue;
}

QLayout* DSceneEdgeElectrostaticMarker::createContent()
{
    cmbType = new QComboBox();
    cmbType->addItem("none", PHYSICFIELDBC_NONE);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_ELECTROSTATIC_POTENTIAL), PHYSICFIELDBC_ELECTROSTATIC_POTENTIAL);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_ELECTROSTATIC_SURFACE_CHARGE), PHYSICFIELDBC_ELECTROSTATIC_SURFACE_CHARGE);

    txtValue = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("BC Type:"), cmbType);
    layoutMarker->addRow(tr("Value:"), txtValue);

    return layoutMarker;
}

void DSceneEdgeElectrostaticMarker::load()
{
    DSceneEdgeMarker::load();

    SceneEdgeElectrostaticMarker *edgeElectrostaticMarker = dynamic_cast<SceneEdgeElectrostaticMarker *>(m_edgeMarker);

    cmbType->setCurrentIndex(cmbType->findData(edgeElectrostaticMarker->type));
    txtValue->setText(QString::number(edgeElectrostaticMarker->value));
}

void DSceneEdgeElectrostaticMarker::save() {
    DSceneEdgeMarker::save();

    SceneEdgeElectrostaticMarker *edgeElectrostaticMarker = dynamic_cast<SceneEdgeElectrostaticMarker *>(m_edgeMarker);

    edgeElectrostaticMarker->type = (PhysicFieldBC) cmbType->itemData(cmbType->currentIndex()).toInt();
    edgeElectrostaticMarker->value = txtValue->text().toDouble();
}

// *************************************************************************************************************************************

DSceneEdgeMagnetostaticMarker::DSceneEdgeMagnetostaticMarker(SceneEdgeMagnetostaticMarker *edgeMagnetostaticMarker, QWidget *parent) : DSceneEdgeMarker(parent)
{
    this->m_edgeMarker = edgeMagnetostaticMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Boundary Condition"));

    createDialog();

    load();
}

DSceneEdgeMagnetostaticMarker::~DSceneEdgeMagnetostaticMarker()
{
    delete cmbType;
    delete txtValue;
}

QLayout* DSceneEdgeMagnetostaticMarker::createContent()
{
    cmbType = new QComboBox();
    cmbType->addItem("none", PHYSICFIELDBC_NONE);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_MAGNETOSTATIC_VECTOR_POTENTIAL), PHYSICFIELDBC_MAGNETOSTATIC_VECTOR_POTENTIAL);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_MAGNETOSTATIC_SURFACE_CURRENT), PHYSICFIELDBC_MAGNETOSTATIC_SURFACE_CURRENT);

    txtValue = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("BC Type:"), cmbType);
    layoutMarker->addRow(tr("Value:"), txtValue);

    return layoutMarker;
}

void DSceneEdgeMagnetostaticMarker::load()
{
    DSceneEdgeMarker::load();

    SceneEdgeMagnetostaticMarker *edgeMagnetostaticMarker = dynamic_cast<SceneEdgeMagnetostaticMarker *>(m_edgeMarker);

    cmbType->setCurrentIndex(cmbType->findData(edgeMagnetostaticMarker->type));
    txtValue->setText(QString::number(edgeMagnetostaticMarker->value));
}

void DSceneEdgeMagnetostaticMarker::save() {
    DSceneEdgeMarker::save();

    SceneEdgeMagnetostaticMarker *edgeMagnetostaticMarker = dynamic_cast<SceneEdgeMagnetostaticMarker *>(m_edgeMarker);

    edgeMagnetostaticMarker->type = (PhysicFieldBC) cmbType->itemData(cmbType->currentIndex()).toInt();
    edgeMagnetostaticMarker->value = txtValue->text().toDouble();
}

// *************************************************************************************************************************************

DSceneEdgeHeatMarker::DSceneEdgeHeatMarker(SceneEdgeHeatMarker *edgeEdgeHeatMarker, QWidget *parent) : DSceneEdgeMarker(parent)
{
    this->m_edgeMarker = edgeEdgeHeatMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Boundary Condition"));

    createDialog();

    load();
}

DSceneEdgeHeatMarker::~DSceneEdgeHeatMarker()
{
    delete cmbType;
    delete txtTemperature;
    delete txtHeatFlux;
    delete txtHeatTransferCoefficient;
    delete txtExternalTemperature;
}

QLayout* DSceneEdgeHeatMarker::createContent()
{
    cmbType = new QComboBox();
    cmbType->addItem("none", PHYSICFIELDBC_NONE);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_HEAT_TEMPERATURE), PHYSICFIELDBC_HEAT_TEMPERATURE);
    cmbType->addItem(physicFieldBCString(PHYSICFIELDBC_HEAT_HEAT_FLUX), PHYSICFIELDBC_HEAT_HEAT_FLUX);
    connect(cmbType, SIGNAL(currentIndexChanged(int)), this, SLOT(doTypeChanged(int)));

    txtHeatFlux = new SLineEdit("0", true);
    txtTemperature = new SLineEdit("0", true);
    txtHeatTransferCoefficient = new SLineEdit("0", true);
    txtExternalTemperature = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("BC Type:"), cmbType);
    layoutMarker->addRow(tr("Temperature:"), txtTemperature);
    layoutMarker->addRow(tr("Heat flux:"), txtHeatFlux);
    layoutMarker->addRow(tr("Heat transfer coef.:"), txtHeatTransferCoefficient);
    layoutMarker->addRow(tr("External temperature:"), txtExternalTemperature);

    return layoutMarker;
}

void DSceneEdgeHeatMarker::load()
{
    DSceneEdgeMarker::load();

    SceneEdgeHeatMarker *edgeHeatMarker = dynamic_cast<SceneEdgeHeatMarker *>(m_edgeMarker);

    cmbType->setCurrentIndex(cmbType->findData(edgeHeatMarker->type));
    switch (edgeHeatMarker->type)
    {
        case PHYSICFIELDBC_HEAT_TEMPERATURE:
        {
            txtTemperature->setText(QString::number(edgeHeatMarker->temperature));
        }
        break;
        case PHYSICFIELDBC_HEAT_HEAT_FLUX:
        {
            txtHeatFlux->setText(QString::number(edgeHeatMarker->heatFlux));
            txtHeatTransferCoefficient->setText(QString::number(edgeHeatMarker->h));
            txtExternalTemperature->setText(QString::number(edgeHeatMarker->externalTemperature));
        }
        break;
    }
}

void DSceneEdgeHeatMarker::save() {
    DSceneEdgeMarker::save();

    SceneEdgeHeatMarker *edgeHeatMarker = dynamic_cast<SceneEdgeHeatMarker *>(m_edgeMarker);

    edgeHeatMarker->type = (PhysicFieldBC) cmbType->itemData(cmbType->currentIndex()).toInt();
    switch (edgeHeatMarker->type)
    {
        case PHYSICFIELDBC_HEAT_TEMPERATURE:
        {
            edgeHeatMarker->temperature = txtTemperature->text().toDouble();
        }
        break;
        case PHYSICFIELDBC_HEAT_HEAT_FLUX:
        {
            edgeHeatMarker->heatFlux = txtHeatFlux->text().toDouble();
            edgeHeatMarker->h = txtHeatTransferCoefficient->text().toDouble();
            edgeHeatMarker->externalTemperature = txtExternalTemperature->text().toDouble();
        }
        break;
    }
}

void DSceneEdgeHeatMarker::doTypeChanged(int index)
{
    txtTemperature->setEnabled(false);
    txtHeatFlux->setEnabled(false);
    txtHeatTransferCoefficient->setEnabled(false);
    txtExternalTemperature->setEnabled(false);

    switch ((PhysicFieldBC) cmbType->itemData(index).toInt())
    {
        case PHYSICFIELDBC_HEAT_TEMPERATURE:
        {
            txtTemperature->setEnabled(true);
        }
        break;
        case PHYSICFIELDBC_HEAT_HEAT_FLUX:
        {
            txtHeatFlux->setEnabled(true);
            txtHeatTransferCoefficient->setEnabled(true);
            txtExternalTemperature->setEnabled(true);
        }
        break;
    }
}

// *************************************************************************************************************************************

DSceneEdgeElasticityMarker::DSceneEdgeElasticityMarker(SceneEdgeElasticityMarker *edgeEdgeElasticityMarker, QWidget *parent) : DSceneEdgeMarker(parent)
{
    this->m_edgeMarker = edgeEdgeElasticityMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Boundary Condition"));

    createDialog();

    load();
}

DSceneEdgeElasticityMarker::~DSceneEdgeElasticityMarker()
{
    delete cmbTypeX;
    delete cmbTypeY;
    delete txtForceX;
    delete txtForceY;
}

QLayout* DSceneEdgeElasticityMarker::createContent()
{
    cmbTypeX = new QComboBox();
    cmbTypeX->addItem("none", PHYSICFIELDBC_NONE);
    cmbTypeX->addItem(physicFieldBCString(PHYSICFIELDBC_ELASTICITY_FREE), PHYSICFIELDBC_ELASTICITY_FREE);
    cmbTypeX->addItem(physicFieldBCString(PHYSICFIELDBC_ELASTICITY_FIXED), PHYSICFIELDBC_ELASTICITY_FIXED);

    cmbTypeY = new QComboBox();
    cmbTypeY->addItem("none", PHYSICFIELDBC_NONE);
    cmbTypeY->addItem(physicFieldBCString(PHYSICFIELDBC_ELASTICITY_FREE), PHYSICFIELDBC_ELASTICITY_FREE);
    cmbTypeY->addItem(physicFieldBCString(PHYSICFIELDBC_ELASTICITY_FIXED), PHYSICFIELDBC_ELASTICITY_FIXED);

    txtForceX = new SLineEdit("0", true);
    txtForceY = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("BC Type X:"), cmbTypeX);
    layoutMarker->addRow(tr("BC Type Y:"), cmbTypeY);
    layoutMarker->addRow(tr("Force X:"), txtForceX);
    layoutMarker->addRow(tr("Force Y:"), txtForceY);

    return layoutMarker;
}

void DSceneEdgeElasticityMarker::load()
{
    DSceneEdgeMarker::load();

    SceneEdgeElasticityMarker *edgeElasticityMarker = dynamic_cast<SceneEdgeElasticityMarker *>(m_edgeMarker);

    cmbTypeX->setCurrentIndex(cmbTypeX->findData(edgeElasticityMarker->typeX));
    cmbTypeY->setCurrentIndex(cmbTypeY->findData(edgeElasticityMarker->typeY));

    txtForceX->setText(QString::number(edgeElasticityMarker->forceX));
    txtForceY->setText(QString::number(edgeElasticityMarker->forceY));
}

void DSceneEdgeElasticityMarker::save() {
    DSceneEdgeMarker::save();

    SceneEdgeElasticityMarker *edgeElasticityMarker = dynamic_cast<SceneEdgeElasticityMarker *>(m_edgeMarker);

    edgeElasticityMarker->typeX = (PhysicFieldBC) cmbTypeX->itemData(cmbTypeX->currentIndex()).toInt();
    edgeElasticityMarker->typeY = (PhysicFieldBC) cmbTypeY->itemData(cmbTypeY->currentIndex()).toInt();
    edgeElasticityMarker->forceX = txtForceX->text().toDouble();
    edgeElasticityMarker->forceY = txtForceY->text().toDouble();
}

// *************************************************************************************************************************************

DSceneLabelMarker::DSceneLabelMarker(QWidget *parent) : QDialog(parent)
{
    layout = new QVBoxLayout();
    txtName = new QLineEdit("");
}

DSceneLabelMarker::~DSceneLabelMarker()
{
    delete layout;
    delete txtName;
}

void DSceneLabelMarker::createDialog()
{
    QHBoxLayout *layoutName = new QHBoxLayout();
    layoutName->addWidget(new QLabel(tr("Name:")));
    layoutName->addWidget(txtName);

    // dialog buttons
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(doAccept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(doReject()));

    layout->addLayout(layoutName);
    layout->addLayout(createContent());
    layout->addStretch();
    layout->addWidget(buttonBox);

    setLayout(layout);
}

void DSceneLabelMarker::load()
{
    txtName->setText(m_labelMarker->name);
}

void DSceneLabelMarker::save()
{
    m_labelMarker->name = txtName->text();
}

void DSceneLabelMarker::doAccept()
{
    save();

    accept();
}

void DSceneLabelMarker::doReject()
{
    reject();
}

// *************************************************************************************************************************************

DSceneLabelElectrostaticMarker::DSceneLabelElectrostaticMarker(QWidget *parent, SceneLabelElectrostaticMarker *labelElectrostaticMarker)
    : DSceneLabelMarker(parent)
{
    this->m_labelMarker = labelElectrostaticMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Material"));

    createDialog();

    load();
}

DSceneLabelElectrostaticMarker::~DSceneLabelElectrostaticMarker()
{
    delete txtPermittivity;
    delete txtChargeDensity;
}

QLayout* DSceneLabelElectrostaticMarker::createContent()
{
    txtPermittivity = new SLineEdit("0", true);
    txtChargeDensity = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("Permittivity:"), txtPermittivity);
    layoutMarker->addRow(tr("Charge Density:"), txtChargeDensity);

    return layoutMarker;
}

void DSceneLabelElectrostaticMarker::load()
{
    DSceneLabelMarker::load();

    SceneLabelElectrostaticMarker *labelElectrostaticMarker = dynamic_cast<SceneLabelElectrostaticMarker *>(m_labelMarker);

    txtPermittivity->setText(QString::number(labelElectrostaticMarker->permittivity));
    txtChargeDensity->setText(QString::number(labelElectrostaticMarker->charge_density));
}

void DSceneLabelElectrostaticMarker::save() {
    DSceneLabelMarker::save();

    SceneLabelElectrostaticMarker *labelElectrostaticMarker = dynamic_cast<SceneLabelElectrostaticMarker *>(m_labelMarker);

    labelElectrostaticMarker->permittivity = txtPermittivity->text().toDouble();
    labelElectrostaticMarker->charge_density = txtChargeDensity->text().toDouble();
}

// *************************************************************************************************************************************

DSceneLabelMagnetostaticMarker::DSceneLabelMagnetostaticMarker(QWidget *parent, SceneLabelMagnetostaticMarker *labelMagnetostaticMarker)
    : DSceneLabelMarker(parent)
{
    this->m_labelMarker = labelMagnetostaticMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Material"));

    createDialog();

    load();
}

DSceneLabelMagnetostaticMarker::~DSceneLabelMagnetostaticMarker()
{
    delete txtPermeability;
    delete txtCurrentDensity;
}

QLayout* DSceneLabelMagnetostaticMarker::createContent()
{
    txtPermeability= new SLineEdit("0", true);
    txtCurrentDensity = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("Permeability:"), txtPermeability);
    layoutMarker->addRow(tr("Current Density:"), txtCurrentDensity);

    return layoutMarker;
}

void DSceneLabelMagnetostaticMarker::load()
{
    DSceneLabelMarker::load();

    SceneLabelMagnetostaticMarker *labelMagnetostaticMarker = dynamic_cast<SceneLabelMagnetostaticMarker *>(m_labelMarker);

    txtPermeability->setText(QString::number(labelMagnetostaticMarker->permeability));
    txtCurrentDensity->setText(QString::number(labelMagnetostaticMarker->current_density));
}

void DSceneLabelMagnetostaticMarker::save() {
    DSceneLabelMarker::save();

    SceneLabelMagnetostaticMarker *labelMagnetostaticMarker = dynamic_cast<SceneLabelMagnetostaticMarker *>(m_labelMarker);

    labelMagnetostaticMarker->permeability = txtPermeability->text().toDouble();
    labelMagnetostaticMarker->current_density = txtCurrentDensity->text().toDouble();
}

// *************************************************************************************************************************************

DSceneLabelHeatMarker::DSceneLabelHeatMarker(QWidget *parent, SceneLabelHeatMarker *labelHeatMarker)
    : DSceneLabelMarker(parent)
{
    this->m_labelMarker = labelHeatMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Material"));

    createDialog();

    load();
}

DSceneLabelHeatMarker::~DSceneLabelHeatMarker()
{
    delete txtThermalConductivity;
    delete txtVolumeHeat;
}

QLayout* DSceneLabelHeatMarker::createContent()
{
    txtThermalConductivity= new SLineEdit("0", true);
    txtVolumeHeat = new SLineEdit("0", true);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("Thermal Conductivity:"), txtThermalConductivity);
    layoutMarker->addRow(tr("Volume Heat:"), txtVolumeHeat);

    return layoutMarker;
}

void DSceneLabelHeatMarker::load()
{
    DSceneLabelMarker::load();

    SceneLabelHeatMarker *labelHeatMarker = dynamic_cast<SceneLabelHeatMarker *>(m_labelMarker);

    txtThermalConductivity->setText(QString::number(labelHeatMarker->thermal_conductivity));
    txtVolumeHeat->setText(QString::number(labelHeatMarker->volume_heat));
}

void DSceneLabelHeatMarker::save()
{
    DSceneLabelMarker::save();

    SceneLabelHeatMarker *labelHeatMarker = dynamic_cast<SceneLabelHeatMarker *>(m_labelMarker);

    labelHeatMarker->thermal_conductivity = txtThermalConductivity->text().toDouble();
    labelHeatMarker->volume_heat = txtVolumeHeat->text().toDouble();
}

// *************************************************************************************************************************************

DSceneLabelElasticityMarker::DSceneLabelElasticityMarker(QWidget *parent, SceneLabelElasticityMarker *labelElasticityMarker)
    : DSceneLabelMarker(parent)
{
    this->m_labelMarker = labelElasticityMarker;

    setMinimumSize(300, 160);
    setWindowTitle(tr("Material"));

    createDialog();

    load();
}

DSceneLabelElasticityMarker::~DSceneLabelElasticityMarker()
{
    delete txtYoungModulus;
    delete txtPoissonNumber;
}

QLayout* DSceneLabelElasticityMarker::createContent()
{
    txtYoungModulus= new SLineEdit("0", 0);
    txtPoissonNumber = new SLineEdit("0", 0);

    QFormLayout *layoutMarker = new QFormLayout();
    layoutMarker->addRow(tr("Young modulus:"), txtYoungModulus);
    layoutMarker->addRow(tr("Poisson number:"), txtPoissonNumber);

    return layoutMarker;
}

void DSceneLabelElasticityMarker::load()
{
    DSceneLabelMarker::load();

    SceneLabelElasticityMarker *labelElasticityMarker = dynamic_cast<SceneLabelElasticityMarker *>(m_labelMarker);

    txtYoungModulus->setText(QString::number(labelElasticityMarker->young_modulus));
    txtPoissonNumber->setText(QString::number(labelElasticityMarker->poisson_ratio));
}

void DSceneLabelElasticityMarker::save()
{
    DSceneLabelMarker::save();

    SceneLabelElasticityMarker *labelElasticityMarker = dynamic_cast<SceneLabelElasticityMarker *>(m_labelMarker);

    labelElasticityMarker->young_modulus = txtYoungModulus->text().toDouble();
    labelElasticityMarker->poisson_ratio = txtPoissonNumber->text().toDouble();
}
