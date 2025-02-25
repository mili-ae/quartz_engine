#include <vector>
#include "engine.cpp"

Texture brickTexture;
Texture concreteTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;


Model *mikuModel;
Model *seahawkModel;

GLfloat seahawkAngle = 0.0f;
GLfloat mikuAngle = 0.0f;

GameObject *miku;
GameObject *seahawk;
GameObject *floorObject;

unsigned int floorIndices[] =
{
    0, 2, 1,
    1, 2, 3
};

GLfloat floorVertices[] =
{
    -10.0f, 0.0f, -10.0f,       0.0f,   0.0f,       0.0f, -1.0f, 0.0f,
    10.0f,  0.0f, -10.0f,       10.0f,  0.0f,       0.0f, -1.0f, 0.0f,
    -10.0f, 0.0f,  10.0f,       0.0f,  10.0f,       0.0f, -1.0f, 0.0f,
    10.0f,  0.0f,  10.0f,       10.0f, 10.0f,       0.0f, -1.0f, 0.0f
};

void updateFloor()
{
    floorObject->transform.matrixModel = glm::translate(floorObject->transform.matrixModel, glm::vec3(0.0f, -2.0f, 0.0f));
}

void updateMiku()
{
    mikuAngle += 15.0f * deltaTime;
    if (mikuAngle > 360.0f) mikuAngle = 0.1f;

    miku->transform.matrixModel = glm::rotate(miku->transform.matrixModel, -mikuAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    miku->transform.matrixModel = glm::translate(miku->transform.matrixModel, glm::vec3(0.0f, -2.0f, 0.0f));
    miku->transform.matrixModel = glm::scale(miku->transform.matrixModel, glm::vec3(0.06f, 0.06f, 0.06f));
}

void updateSeahawk()
{
    seahawkAngle += 15.0f * deltaTime;
    if (seahawkAngle > 360.0f) seahawkAngle = 0.1f;

    seahawk->transform.matrixModel = glm::rotate(seahawk->transform.matrixModel, -seahawkAngle * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    seahawk->transform.matrixModel = glm::translate(seahawk->transform.matrixModel, glm::vec3(4.0f, 1.0f, 0.0f));
    seahawk->transform.matrixModel = glm::rotate(seahawk->transform.matrixModel, 20.0f * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
    seahawk->transform.matrixModel = glm::scale(seahawk->transform.matrixModel, glm::vec3(0.02f, 0.02f, 0.02f));
}

int main(int argc, char *args[])
{
    init();
    projection = glm::perspective(glm::radians(60.0f), (GLfloat)window.bufferWidth / window.bufferHeight, 0.1f, 100.0f);

    shinyMaterial = Material(1.0f, 32);
    dullMaterial = Material(0.3f, 4);

    mikuModel = new Model();
    mikuModel->load("assets/models/mecha_miku/scene.gltf");
    seahawkModel = new Model();
    seahawkModel->load("assets/models/seahawk/Seahawk.obj");

    Mesh *floorMesh = new Mesh();
    floorMesh->create(floorVertices, floorIndices, 32, 6);
    floorObject = new GameObject();
    floorObject->addMesh(floorMesh);
    floorObject->addMaterial(shinyMaterial);
    floorObject->addTexture("assets/textures/cracked_concrete.jpg");
    floorObject->setUpdate(updateFloor);
    
    
    miku = new GameObject();
    miku->addModel(mikuModel);
    miku->addMaterial(shinyMaterial);
    miku->setUpdate(updateMiku);

    seahawk = new GameObject();
    seahawk->addModel(seahawkModel);
    seahawk->addMaterial(shinyMaterial);
    seahawk->setUpdate(updateSeahawk);
    
    gameobjects.push_back(floorObject);
    gameobjects.push_back(miku);
    // gameobjects.push_back(seahawk);

    std::vector<std::string> skyboxFaces;

    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_E.png");
    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_W.png");
    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_U.png");
    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_D.png");
    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_N.png");
    skyboxFaces.push_back("assets/textures/sunset_bay/SunsetBay_S.png");

    skybox = Skybox(skyboxFaces);

    update();

    delete miku;
    delete seahawk;

    return 0;
}