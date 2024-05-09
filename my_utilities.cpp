#include "my_utilities.h"
#include <glm/glm.hpp>
#include <iostream>

// Initialize each buttons position based on the number of colors needed
// Might be a little bit too long for initializing the vertices and indices for the arrow buttons. Can be more efficient.
void MyUtilities::initializeArrows()
{
    MyModel::Vertex boxTopLeft1;  // index 0 
    boxTopLeft1.position = glm::vec3(A_LEFT, A_BOTTOM + HEIGHT, 5.0f);
    boxTopLeft1.color = GRAY;  
    m_vButtons.push_back(boxTopLeft1);

    MyModel::Vertex boxTopRight1; // index 1
    boxTopRight1.position = glm::vec3(A_RIGHT, A_BOTTOM + HEIGHT, 5.0f);
    boxTopRight1.color = GRAY;  
    m_vButtons.push_back(boxTopRight1);

    MyModel::Vertex boxBottomLeft1; // index 2
    boxBottomLeft1.position = glm::vec3(A_LEFT, A_BOTTOM, 5.0f);
    boxBottomLeft1.color = GRAY;  
    m_vButtons.push_back(boxBottomLeft1);

    MyModel::Vertex boxBottomRight1; // index 3
    boxBottomRight1.position = glm::vec3(A_RIGHT, A_BOTTOM, 5.0f);
    boxBottomRight1.color = GRAY;  
    m_vButtons.push_back(boxBottomRight1);

    m_vIndices1.push_back(0);
    m_vIndices1.push_back(2);
    m_vIndices1.push_back(3);
    m_vIndices1.push_back(0);
    m_vIndices1.push_back(1);
    m_vIndices1.push_back(3);

    MyModel::Vertex arrowPoint1; // index 4
    arrowPoint1.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f), A_BOTTOM + ARROW_GAP, 9.0f);
    arrowPoint1.color = WHITE;  
    m_vButtons.push_back(arrowPoint1);

    MyModel::Vertex arrowPoint2; // index 5
    arrowPoint2.position = glm::vec3(A_LEFT + ARROW_GAP, A_BOTTOM + (HEIGHT / 2.0f), 9.0f);
    arrowPoint2.color = WHITE;  
    m_vButtons.push_back(arrowPoint2);

    MyModel::Vertex arrowPoint3; // index 6
    arrowPoint3.position = glm::vec3(A_RIGHT - ARROW_GAP, A_BOTTOM + (HEIGHT / 2.0f), 9.0f);
    arrowPoint3.color = WHITE;  
    m_vButtons.push_back(arrowPoint3);

    m_vIndices1.push_back(4);
    m_vIndices1.push_back(5);
    m_vIndices1.push_back(6);

    MyModel::Vertex arrowTopLeft1; // index 7
    arrowTopLeft1.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) - (ARROW_WIDTH / 2.0f), A_BOTTOM + HEIGHT - ARROW_GAP, 9.0f);
    arrowTopLeft1.color = WHITE;  
    m_vButtons.push_back(arrowTopLeft1);

    MyModel::Vertex arrowTopRight1; // index 8
    arrowTopRight1.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) + (ARROW_WIDTH / 2.0f), A_BOTTOM + HEIGHT - ARROW_GAP, 9.0f);
    arrowTopRight1.color = WHITE;  
    m_vButtons.push_back(arrowTopRight1);

    MyModel::Vertex arrowBottomLeft1; // index 9
    arrowBottomLeft1.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) - (ARROW_WIDTH / 2.0f), A_BOTTOM + (HEIGHT / 2.0f), 9.0f);
    arrowBottomLeft1.color = WHITE;  
    m_vButtons.push_back(arrowBottomLeft1);

    MyModel::Vertex arrowBottomRight1; // index 10
    arrowBottomRight1.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) + (ARROW_WIDTH / 2.0f), A_BOTTOM + (HEIGHT / 2.0f), 9.0f);
    arrowBottomRight1.color = WHITE;  
    m_vButtons.push_back(arrowBottomRight1);

    m_vIndices1.push_back(7);
    m_vIndices1.push_back(9);
    m_vIndices1.push_back(10);
    m_vIndices1.push_back(7);
    m_vIndices1.push_back(8);
    m_vIndices1.push_back(10);

    MyModel::Vertex boxTopLeft2;  // index 11
    boxTopLeft2.position = glm::vec3(A_LEFT, A_TOP, 5.0f);
    boxTopLeft2.color = GRAY;  
    m_vButtons.push_back(boxTopLeft2);

    MyModel::Vertex boxTopRight2; // index 12
    boxTopRight2.position = glm::vec3(A_RIGHT, A_TOP, 5.0f);
    boxTopRight2.color = GRAY;  
    m_vButtons.push_back(boxTopRight2);

    MyModel::Vertex boxBottomLeft2; // index 13
    boxBottomLeft2.position = glm::vec3(A_LEFT, A_TOP - HEIGHT, 5.0f);
    boxBottomLeft2.color = GRAY;  
    m_vButtons.push_back(boxBottomLeft2);

    MyModel::Vertex boxBottomRight2; // index 14
    boxBottomRight2.position = glm::vec3(A_RIGHT, A_TOP - HEIGHT, 5.0f);
    boxBottomRight2.color = GRAY;  
    m_vButtons.push_back(boxBottomRight2);

    m_vIndices1.push_back(11);
    m_vIndices1.push_back(13);
    m_vIndices1.push_back(14);
    m_vIndices1.push_back(11);
    m_vIndices1.push_back(12);
    m_vIndices1.push_back(14);

    MyModel::Vertex arrowPoint4; // index 15
    arrowPoint4.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f), A_TOP - ARROW_GAP, 9.0f);
    arrowPoint4.color = WHITE;  
    m_vButtons.push_back(arrowPoint4);

    MyModel::Vertex arrowPoint5; // index 16
    arrowPoint5.position = glm::vec3(A_LEFT + ARROW_GAP, A_TOP - (HEIGHT / 2.0f), 9.0f);
    arrowPoint5.color = WHITE;  
    m_vButtons.push_back(arrowPoint5);

    MyModel::Vertex arrowPoint6; // index 17
    arrowPoint6.position = glm::vec3(A_RIGHT - ARROW_GAP, A_TOP - (HEIGHT / 2.0f), 9.0f);
    arrowPoint6.color = WHITE;  
    m_vButtons.push_back(arrowPoint6);

    m_vIndices1.push_back(15);
    m_vIndices1.push_back(16);
    m_vIndices1.push_back(17);

    MyModel::Vertex arrowTopLeft2; // index 18
    arrowTopLeft2.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) - (ARROW_WIDTH / 2.0f), A_TOP - HEIGHT + ARROW_GAP, 9.0f);
    arrowTopLeft2.color = WHITE;  
    m_vButtons.push_back(arrowTopLeft2);

    MyModel::Vertex arrowTopRight2; // index 19
    arrowTopRight2.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) + (ARROW_WIDTH / 2.0f), A_TOP - HEIGHT + ARROW_GAP, 9.0f);
    arrowTopRight2.color = WHITE;  
    m_vButtons.push_back(arrowTopRight2);

    MyModel::Vertex arrowBottomLeft2; // index 20
    arrowBottomLeft2.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) - (ARROW_WIDTH / 2.0f), A_TOP - (HEIGHT / 2.0f), 9.0f);
    arrowBottomLeft2.color = WHITE;  
    m_vButtons.push_back(arrowBottomLeft2);

    MyModel::Vertex arrowBottomRight2; // index 21
    arrowBottomRight2.position = glm::vec3(((A_LEFT + A_RIGHT) / 2.0f) + (ARROW_WIDTH / 2.0f), A_TOP - (HEIGHT / 2.0f), 9.0f);
    arrowBottomRight2.color = WHITE;  
    m_vButtons.push_back(arrowBottomRight2);

    m_vIndices1.push_back(18);
    m_vIndices1.push_back(20);
    m_vIndices1.push_back(21);
    m_vIndices1.push_back(18);
    m_vIndices1.push_back(19);
    m_vIndices1.push_back(21);
};

// Might be a little bit too long for initializing the vertices and indices for the range buttons. Can be more efficient
void MyUtilities::initializeRange()
{
    float TOP_BOX_MID = (R_TOP + ((R_TOP + R_BOTTOM) / 2.0f) + (GAP / 2.0f)) / 2.0f; 
    float BOTTOM_BOX_MID = (R_BOTTOM + ((R_TOP + R_BOTTOM) / 2.0f) - (GAP / 2.0f)) / 2.0f;

    MyModel::Vertex x1;  // index 0
    x1.position = glm::vec3(R_LEFT + GAP, TOP_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    x1.color = WHITE;  
    m_vRange.push_back(x1);

    MyModel::Vertex x2;  // index 1
    x2.position = glm::vec3(R_LEFT + GAP + STROKE_WIDTH, TOP_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    x2.color = WHITE;  
    m_vRange.push_back(x2);

    MyModel::Vertex x3;  // index 2
    x3.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP, TOP_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    x3.color = WHITE;  
    m_vRange.push_back(x3);

    MyModel::Vertex x4;  // index 3
    x4.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP + STROKE_WIDTH, TOP_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    x4.color = WHITE;  
    m_vRange.push_back(x4);

    m_vIndices2.push_back(0);
    m_vIndices2.push_back(1);
    m_vIndices2.push_back(3);
    m_vIndices2.push_back(0);
    m_vIndices2.push_back(2);
    m_vIndices2.push_back(3);

    MyModel::Vertex x5;  // index 4
    x5.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP, TOP_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    x5.color = WHITE;  
    m_vRange.push_back(x5);

    MyModel::Vertex x6;  // index 5
    x6.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP + STROKE_WIDTH, TOP_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    x6.color = WHITE;  
    m_vRange.push_back(x6);

    MyModel::Vertex x7;  // index 6
    x7.position = glm::vec3(R_LEFT + GAP, TOP_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    x7.color = WHITE;  
    m_vRange.push_back(x7);

    MyModel::Vertex x8;  // index 7
    x8.position = glm::vec3(R_LEFT + GAP + STROKE_WIDTH, TOP_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    x8.color = WHITE;  
    m_vRange.push_back(x8);

    m_vIndices2.push_back(4);
    m_vIndices2.push_back(6);
    m_vIndices2.push_back(7);
    m_vIndices2.push_back(4);
    m_vIndices2.push_back(5);
    m_vIndices2.push_back(7);

    MyModel::Vertex y1;  // index 8
    y1.position = glm::vec3(R_LEFT + GAP, BOTTOM_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    y1.color = WHITE;  
    m_vRange.push_back(y1);

    MyModel::Vertex y2;  // index 9
    y2.position = glm::vec3(R_LEFT + GAP + STROKE_WIDTH, BOTTOM_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    y2.color = WHITE;  
    m_vRange.push_back(y2);

    MyModel::Vertex y3;  // index 10
    y3.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f - (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID, 5.0f);
    y3.color = WHITE;  
    m_vRange.push_back(y3);

    MyModel::Vertex y4;  // index 11
    y4.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f, BOTTOM_BOX_MID, 5.0f);
    y4.color = WHITE;  
    m_vRange.push_back(y4);

    m_vIndices2.push_back(8);
    m_vIndices2.push_back(10);
    m_vIndices2.push_back(11);
    m_vIndices2.push_back(8);
    m_vIndices2.push_back(9);
    m_vIndices2.push_back(11);

    MyModel::Vertex y5;  // index 12
    y5.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP, BOTTOM_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    y5.color = WHITE;  
    m_vRange.push_back(y5);

    MyModel::Vertex y6;  // index 13
    y6.position = glm::vec3(R_LEFT + ALPHA_BOX_WIDTH - GAP + STROKE_WIDTH, BOTTOM_BOX_MID + (ALPHA_HEIGHT / 2.0f), 5.0f);
    y6.color = WHITE;  
    m_vRange.push_back(y6);

    MyModel::Vertex y7;  // index 14
    y7.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f + (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID, 5.0f);
    y7.color = WHITE;  
    m_vRange.push_back(y7);

    MyModel::Vertex y8;  // index 15
    y8.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f, BOTTOM_BOX_MID, 5.0f);
    y8.color = WHITE;  
    m_vRange.push_back(y8);

    m_vIndices2.push_back(12);
    m_vIndices2.push_back(14);
    m_vIndices2.push_back(15);
    m_vIndices2.push_back(12);
    m_vIndices2.push_back(13);
    m_vIndices2.push_back(15);

    MyModel::Vertex y9;  // index 16
    y9.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f + (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID, 5.0f);
    y9.color = WHITE;  
    m_vRange.push_back(y9);

    MyModel::Vertex y10;  // index 17
    y10.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f - (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID, 5.0f);
    y10.color = WHITE;  
    m_vRange.push_back(y10);

    MyModel::Vertex y11;  // index 18
    y11.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f + (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    y11.color = WHITE;  
    m_vRange.push_back(y11);

    MyModel::Vertex y12;  // index 19
    y12.position = glm::vec3((2.0f * R_LEFT + ALPHA_BOX_WIDTH + STROKE_WIDTH) / 2.0f - (STROKE_WIDTH / 2.0f), BOTTOM_BOX_MID - (ALPHA_HEIGHT / 2.0f), 5.0f);
    y12.color = WHITE;  
    m_vRange.push_back(y12);

    m_vIndices2.push_back(16);
    m_vIndices2.push_back(18);
    m_vIndices2.push_back(19);
    m_vIndices2.push_back(16);
    m_vIndices2.push_back(17);
    m_vIndices2.push_back(19);

    float TOP_BOX_BOTTOM = (R_TOP + R_BOTTOM) / 2.0f + (GAP / 2.0f);
    float BOTTOM_BOX_TOP = (R_TOP + R_BOTTOM) / 2.0f - (GAP / 2.0f);

    MyModel::Vertex minusTopLeft;  // index 20
    minusTopLeft.position = glm::vec3(BUTTON_START, R_TOP - GAP, 5.0f);
    minusTopLeft.color = GRAY;  
    m_vRange.push_back(minusTopLeft);

    MyModel::Vertex minusTopRight;  // index 21
    minusTopRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH, R_TOP - GAP, 5.0f);
    minusTopRight.color = GRAY;  
    m_vRange.push_back(minusTopRight);

    MyModel::Vertex minusBottomLeft;  // index 22
    minusBottomLeft.position = glm::vec3(BUTTON_START, TOP_BOX_BOTTOM + GAP, 5.0f);
    minusBottomLeft.color = GRAY;  
    m_vRange.push_back(minusBottomLeft);

    MyModel::Vertex minusBottomRight;  // index 23
    minusBottomRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH, TOP_BOX_BOTTOM + GAP, 5.0f);
    minusBottomRight.color = GRAY;  
    m_vRange.push_back(minusBottomRight);

    m_vIndices2.push_back(20);
    m_vIndices2.push_back(22);
    m_vIndices2.push_back(23);
    m_vIndices2.push_back(20);
    m_vIndices2.push_back(21);
    m_vIndices2.push_back(23);

    MyModel::Vertex plusTopLeft;  // index 24
    plusTopLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH, R_TOP - GAP, 5.0f);
    plusTopLeft.color = GRAY;  
    m_vRange.push_back(plusTopLeft);

    MyModel::Vertex plusTopRight;  // index 25
    plusTopRight.position = glm::vec3(R_RIGHT, R_TOP - GAP, 5.0f);
    plusTopRight.color = GRAY;  
    m_vRange.push_back(plusTopRight);

    MyModel::Vertex plusBottomLeft;  // index 26
    plusBottomLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH, TOP_BOX_BOTTOM + GAP, 5.0f);
    plusBottomLeft.color = GRAY;  
    m_vRange.push_back(plusBottomLeft);

    MyModel::Vertex plusBottomRight;  // index 27
    plusBottomRight.position = glm::vec3(R_RIGHT, TOP_BOX_BOTTOM + GAP, 5.0f);
    plusBottomRight.color = GRAY;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(24);
    m_vIndices2.push_back(26);
    m_vIndices2.push_back(27);
    m_vIndices2.push_back(24);
    m_vIndices2.push_back(25);
    m_vIndices2.push_back(27);

    // index 28
    minusTopLeft.position = glm::vec3(BUTTON_START, BOTTOM_BOX_TOP - GAP, 5.0f);
    minusTopLeft.color = GRAY;  
    m_vRange.push_back(minusTopLeft);

    // index 29
    minusTopRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH, BOTTOM_BOX_TOP - GAP, 5.0f);
    minusTopRight.color = GRAY;  
    m_vRange.push_back(minusTopRight);

    // index 30
    minusBottomLeft.position = glm::vec3(BUTTON_START, R_BOTTOM + GAP, 5.0f);
    minusBottomLeft.color = GRAY;  
    m_vRange.push_back(minusBottomLeft);

    // index 31
    minusBottomRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH, R_BOTTOM + GAP, 5.0f);
    minusBottomRight.color = GRAY;  
    m_vRange.push_back(minusBottomRight);

    m_vIndices2.push_back(28);
    m_vIndices2.push_back(30);
    m_vIndices2.push_back(31);
    m_vIndices2.push_back(28);
    m_vIndices2.push_back(29);
    m_vIndices2.push_back(31);

    // index 32
    plusTopLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH, BOTTOM_BOX_TOP - GAP, 5.0f);
    plusTopLeft.color = GRAY;  
    m_vRange.push_back(plusTopLeft);

    // index 33
    plusTopRight.position = glm::vec3(R_RIGHT, BOTTOM_BOX_TOP - GAP, 5.0f);
    plusTopRight.color = GRAY;  
    m_vRange.push_back(plusTopRight);

    // index 34
    plusBottomLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH, R_BOTTOM + GAP, 5.0f);
    plusBottomLeft.color = GRAY;  
    m_vRange.push_back(plusBottomLeft);

    // index 35
    plusBottomRight.position = glm::vec3(R_RIGHT, R_BOTTOM + GAP, 5.0f);
    plusBottomRight.color = GRAY;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(32);
    m_vIndices2.push_back(34);
    m_vIndices2.push_back(35);
    m_vIndices2.push_back(32);
    m_vIndices2.push_back(33);
    m_vIndices2.push_back(35);

    // index 36
    minusTopLeft.position = glm::vec3(BUTTON_START + GAP, TOP_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    minusTopLeft.color = WHITE;  
    m_vRange.push_back(minusTopLeft);

    // index 37
    minusTopRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH - GAP, TOP_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    minusTopRight.color = WHITE;  
    m_vRange.push_back(minusTopRight);

    // index 38
    minusBottomLeft.position = glm::vec3(BUTTON_START + GAP, TOP_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    minusBottomLeft.color = WHITE;  
    m_vRange.push_back(minusBottomLeft);

    // index 39
    minusBottomRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH - GAP, TOP_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    minusBottomRight.color = WHITE;  
    m_vRange.push_back(minusBottomRight);

    m_vIndices2.push_back(36);
    m_vIndices2.push_back(38);
    m_vIndices2.push_back(39);
    m_vIndices2.push_back(36);
    m_vIndices2.push_back(37);
    m_vIndices2.push_back(39);

    // index 40
    minusTopLeft.position = glm::vec3(BUTTON_START + GAP, BOTTOM_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    minusTopLeft.color = WHITE;  
    m_vRange.push_back(minusTopLeft);

    // index 41
    minusTopRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH - GAP, BOTTOM_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    minusTopRight.color = WHITE;  
    m_vRange.push_back(minusTopRight);

    // index 42
    minusBottomLeft.position = glm::vec3(BUTTON_START + GAP, BOTTOM_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    minusBottomLeft.color = WHITE;  
    m_vRange.push_back(minusBottomLeft);

    // index 43
    minusBottomRight.position = glm::vec3(BUTTON_START + BUTTON_BOX_WIDTH - GAP, BOTTOM_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    minusBottomRight.color = WHITE;  
    m_vRange.push_back(minusBottomRight);

    m_vIndices2.push_back(40);
    m_vIndices2.push_back(42);
    m_vIndices2.push_back(43);
    m_vIndices2.push_back(40);
    m_vIndices2.push_back(41);
    m_vIndices2.push_back(43);

    // index 44
    plusTopLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH + GAP, TOP_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    plusTopLeft.color = WHITE;  
    m_vRange.push_back(plusTopLeft);

    // index 45
    plusTopRight.position = glm::vec3(R_RIGHT - GAP, TOP_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    plusTopRight.color = WHITE;  
    m_vRange.push_back(plusTopRight);

    // index 46
    plusBottomLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH + GAP, TOP_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    plusBottomLeft.color = WHITE;  
    m_vRange.push_back(plusBottomLeft);

    // index 47
    plusBottomRight.position = glm::vec3(R_RIGHT - GAP, TOP_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    plusBottomRight.color = WHITE;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(44);
    m_vIndices2.push_back(46);
    m_vIndices2.push_back(47);
    m_vIndices2.push_back(44);
    m_vIndices2.push_back(45);
    m_vIndices2.push_back(47);

    // index 48
    plusTopLeft.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) - (STROKE_WIDTH / 2.0f), R_TOP - GAP * 2.0f, 6.0f);
    plusTopLeft.color = WHITE;  
    m_vRange.push_back(plusTopLeft);

    // index 49
    plusTopRight.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) + (STROKE_WIDTH / 2.0f), R_TOP - GAP * 2.0f, 6.0f);
    plusTopRight.color = WHITE;  
    m_vRange.push_back(plusTopRight);

    // index 50
    plusBottomLeft.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) - (STROKE_WIDTH / 2.0f), TOP_BOX_BOTTOM + GAP * 2.0f, 6.0f);
    plusBottomLeft.color = WHITE;  
    m_vRange.push_back(plusBottomLeft);

    // index 51
    plusBottomRight.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) + (STROKE_WIDTH / 2.0f), TOP_BOX_BOTTOM + GAP * 2.0f, 6.0f);
    plusBottomRight.color = WHITE;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(48);
    m_vIndices2.push_back(50);
    m_vIndices2.push_back(51);
    m_vIndices2.push_back(48);
    m_vIndices2.push_back(49);
    m_vIndices2.push_back(51);

    // index 52
    plusTopLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH + GAP, BOTTOM_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    plusTopLeft.color = WHITE;  
    m_vRange.push_back(plusTopLeft);

    // index 53
    plusTopRight.position = glm::vec3(R_RIGHT - GAP, BOTTOM_BOX_MID + (STROKE_WIDTH / 2.0f), 6.0f);
    plusTopRight.color = WHITE;  
    m_vRange.push_back(plusTopRight);

    // index 54
    plusBottomLeft.position = glm::vec3(R_RIGHT - BUTTON_BOX_WIDTH + GAP, BOTTOM_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    plusBottomLeft.color = WHITE;  
    m_vRange.push_back(plusBottomLeft);

    // index 55
    plusBottomRight.position = glm::vec3(R_RIGHT - GAP, BOTTOM_BOX_MID - (STROKE_WIDTH / 2.0f), 6.0f);
    plusBottomRight.color = WHITE;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(52);
    m_vIndices2.push_back(54);
    m_vIndices2.push_back(55);
    m_vIndices2.push_back(52);
    m_vIndices2.push_back(53);
    m_vIndices2.push_back(55);

    // index 56
    plusTopLeft.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) - (STROKE_WIDTH / 2.0f), R_BOTTOM + GAP * 2.0f, 6.0f);
    plusTopLeft.color = WHITE;  
    m_vRange.push_back(plusTopLeft);

    // index 57
    plusTopRight.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) + (STROKE_WIDTH / 2.0f), R_BOTTOM + GAP * 2.0f, 6.0f);
    plusTopRight.color = WHITE;  
    m_vRange.push_back(plusTopRight);

    // index 58
    plusBottomLeft.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) - (STROKE_WIDTH / 2.0f), BOTTOM_BOX_TOP - GAP * 2.0f, 6.0f);
    plusBottomLeft.color = WHITE;  
    m_vRange.push_back(plusBottomLeft);

    // index 59
    plusBottomRight.position = glm::vec3((R_RIGHT + (R_RIGHT - BUTTON_BOX_WIDTH) / 2.0f) + (STROKE_WIDTH / 2.0f), BOTTOM_BOX_TOP - GAP * 2.0f, 6.0f);
    plusBottomRight.color = WHITE;  
    m_vRange.push_back(plusBottomRight);

    m_vIndices2.push_back(56);
    m_vIndices2.push_back(58);
    m_vIndices2.push_back(59);
    m_vIndices2.push_back(56);
    m_vIndices2.push_back(57);
    m_vIndices2.push_back(59);

    float RANGE_WIDTH = ((R_RIGHT - BUTTON_BOX_WIDTH) - (BUTTON_START + BUTTON_BOX_WIDTH) - (RANGE_GAP * float(MAX_DEGREE + 1))) / MAX_DEGREE; 
    float range_start = BUTTON_START + BUTTON_BOX_WIDTH + RANGE_GAP; 
    glm::vec3 color = WHITE; 
    // Index start from 60 and end at 99
    for (int i = 0; i < MAX_DEGREE; i++)
    {
        MyModel::Vertex rangeTopLeft;
        rangeTopLeft.position = glm::vec3(range_start, R_TOP - GAP, 5.0f);
        rangeTopLeft.color = color;  
        m_vRange.push_back(rangeTopLeft);

        MyModel::Vertex rangeTopRight;
        rangeTopRight.position = glm::vec3(range_start + RANGE_WIDTH, R_TOP - GAP, 5.0f);
        rangeTopRight.color = color;  
        m_vRange.push_back(rangeTopRight);

        MyModel::Vertex rangeBottomLeft;
        rangeBottomLeft.position = glm::vec3(range_start, TOP_BOX_BOTTOM + GAP, 5.0f);
        rangeBottomLeft.color = color;  
        m_vRange.push_back(rangeBottomLeft);

        MyModel::Vertex rangeBottomRight;
        rangeBottomRight.position = glm::vec3(range_start + RANGE_WIDTH, TOP_BOX_BOTTOM + GAP, 5.0f);
        rangeBottomRight.color = color;  
        m_vRange.push_back(rangeBottomRight);

        m_vIndices2.push_back(i * 4 + 60);
        m_vIndices2.push_back(i * 4 + 62);
        m_vIndices2.push_back(i * 4 + 63);
        m_vIndices2.push_back(i * 4 + 60);
        m_vIndices2.push_back(i * 4 + 61);
        m_vIndices2.push_back(i * 4 + 63);

        range_start = range_start + RANGE_WIDTH + RANGE_GAP; 

        if (i == u_deg - 1)
        {
            color = BLACK; 
        }
    }

    range_start = BUTTON_START + BUTTON_BOX_WIDTH + RANGE_GAP;
    color = WHITE;

    // Index start from 100 and end at 139
    for (int i = 0; i < MAX_DEGREE; i++)
    {
        MyModel::Vertex rangeTopLeft;
        rangeTopLeft.position = glm::vec3(range_start, BOTTOM_BOX_TOP - GAP, 5.0f);
        rangeTopLeft.color = color;  
        m_vRange.push_back(rangeTopLeft);

        MyModel::Vertex rangeTopRight;
        rangeTopRight.position = glm::vec3(range_start + RANGE_WIDTH, BOTTOM_BOX_TOP - GAP, 5.0f);
        rangeTopRight.color = color;  
        m_vRange.push_back(rangeTopRight);

        MyModel::Vertex rangeBottomLeft;
        rangeBottomLeft.position = glm::vec3(range_start, R_BOTTOM + GAP, 5.0f);
        rangeBottomLeft.color = color;  
        m_vRange.push_back(rangeBottomLeft);

        MyModel::Vertex rangeBottomRight;
        rangeBottomRight.position = glm::vec3(range_start + RANGE_WIDTH, R_BOTTOM + GAP, 5.0f);
        rangeBottomRight.color = color;  
        m_vRange.push_back(rangeBottomRight);

        m_vIndices2.push_back(i * 4 + 100);
        m_vIndices2.push_back(i * 4 + 102);
        m_vIndices2.push_back(i * 4 + 103);
        m_vIndices2.push_back(i * 4 + 100);
        m_vIndices2.push_back(i * 4 + 101);
        m_vIndices2.push_back(i * 4 + 103);

        range_start = range_start + RANGE_WIDTH + RANGE_GAP; 

        if (i == v_deg - 1)
        {
            color = BLACK; 
        }
    }
}

// Initialize each texture buttons' position
void MyUtilities::initializeTexture()
{
    float TOP_BOX_BOTTOM = (R_TOP + R_BOTTOM) / 2.0f + (GAP / 2.0f);
    float BOTTOM_BOX_TOP = (R_TOP + R_BOTTOM) / 2.0f - (GAP / 2.0f);

    MyModel::Vertex textureTopLeft;  // index 0
    textureTopLeft.position = glm::vec3(R_RIGHT + 6.0f * GAP, T_TOP - GAP, 5.0f);
    textureTopLeft.color = WHITE; 
    textureTopLeft.uv = glm::vec2(0.0f, 0.0f);
    m_vTexture.push_back(textureTopLeft);

    MyModel::Vertex textureTopRight;  // index 1
    textureTopRight.position = glm::vec3(R_RIGHT + BOX_WIDTH + 6.0f * GAP, T_TOP - GAP, 5.0f);
    textureTopRight.color = WHITE;  
    textureTopRight.uv = glm::vec2(0.25f, 0.0f);
    m_vTexture.push_back(textureTopRight);

    MyModel::Vertex textureBottomLeft;  // index 2
    textureBottomLeft.position = glm::vec3(R_RIGHT + 6.0f * GAP, TOP_BOX_BOTTOM + GAP, 5.0f);
    textureBottomLeft.color = WHITE;  
    textureBottomLeft.uv = glm::vec2(0.0f, 1.0f);
    m_vTexture.push_back(textureBottomLeft);

    MyModel::Vertex textureBottomRight;  // index 3
    textureBottomRight.position = glm::vec3(R_RIGHT + BOX_WIDTH + 6.0f * GAP, TOP_BOX_BOTTOM + GAP, 5.0f);
    textureBottomRight.color = WHITE;  
    textureBottomRight.uv = glm::vec2(0.25f, 1.0f);
    m_vTexture.push_back(textureBottomRight);

    m_vIndices3.push_back(0);
    m_vIndices3.push_back(1);
    m_vIndices3.push_back(3);
    m_vIndices3.push_back(0);
    m_vIndices3.push_back(2);
    m_vIndices3.push_back(3);

    // index 4
    textureTopLeft.position = glm::vec3(R_RIGHT + BOX_WIDTH + 8.0f * GAP, T_TOP - GAP, 5.0f);
    textureTopLeft.color = WHITE; 
    textureTopLeft.uv = glm::vec2(0.25f, 0.0f);
    m_vTexture.push_back(textureTopLeft);

    // index 5
    textureTopRight.position = glm::vec3(R_RIGHT + 2.0f * BOX_WIDTH + 8.0f * GAP, T_TOP - GAP, 5.0f);
    textureTopRight.color = WHITE;  
    textureTopRight.uv = glm::vec2(0.5f, 0.0f);
    m_vTexture.push_back(textureTopRight);

    // index 6
    textureBottomLeft.position = glm::vec3(R_RIGHT + BOX_WIDTH + 8.0f * GAP, TOP_BOX_BOTTOM + GAP, 5.0f);
    textureBottomLeft.color = WHITE;  
    textureBottomLeft.uv = glm::vec2(0.25f, 1.0f);
    m_vTexture.push_back(textureBottomLeft);

    // index 7
    textureBottomRight.position = glm::vec3(R_RIGHT + 2.0f * BOX_WIDTH + 8.0f * GAP, TOP_BOX_BOTTOM + GAP, 5.0f);
    textureBottomRight.color = WHITE;  
    textureBottomRight.uv = glm::vec2(0.5f, 1.0f);
    m_vTexture.push_back(textureBottomRight);

    m_vIndices3.push_back(4);
    m_vIndices3.push_back(5);
    m_vIndices3.push_back(7);
    m_vIndices3.push_back(4);
    m_vIndices3.push_back(6);
    m_vIndices3.push_back(7);

    // index 8
    textureTopLeft.position = glm::vec3(R_RIGHT + 6.0f * GAP, TOP_BOX_BOTTOM - GAP, 5.0f);
    textureTopLeft.color = WHITE; 
    textureTopLeft.uv = glm::vec2(0.5f, 0.0f);
    m_vTexture.push_back(textureTopLeft);

    // index 9
    textureTopRight.position = glm::vec3(R_RIGHT + BOX_WIDTH + 6.0f * GAP, TOP_BOX_BOTTOM - GAP, 5.0f);
    textureTopRight.color = WHITE;  
    textureTopRight.uv = glm::vec2(0.75f, 0.0f);
    m_vTexture.push_back(textureTopRight);

    // index 10
    textureBottomLeft.position = glm::vec3(R_RIGHT + 6.0f * GAP, 2.0f * TOP_BOX_BOTTOM - T_TOP + GAP, 5.0f);
    textureBottomLeft.color = WHITE;  
    textureBottomLeft.uv = glm::vec2(0.5f, 1.0f);
    m_vTexture.push_back(textureBottomLeft);

    // index 11
    textureBottomRight.position = glm::vec3(R_RIGHT + BOX_WIDTH + 6.0f * GAP, 2.0f * TOP_BOX_BOTTOM - T_TOP + GAP, 5.0f);
    textureBottomRight.color = WHITE;  
    textureBottomRight.uv = glm::vec2(0.75f, 1.0f);
    m_vTexture.push_back(textureBottomRight);

    m_vIndices3.push_back(8);
    m_vIndices3.push_back(9);
    m_vIndices3.push_back(11);
    m_vIndices3.push_back(8);
    m_vIndices3.push_back(10);
    m_vIndices3.push_back(11);

    // index 12
    textureTopLeft.position = glm::vec3(R_RIGHT + BOX_WIDTH + 8.0f * GAP, TOP_BOX_BOTTOM - GAP, 5.0f);
    textureTopLeft.color = WHITE; 
    textureTopLeft.uv = glm::vec2(0.75f, 0.0f);
    m_vTexture.push_back(textureTopLeft);

    // index 13
    textureTopRight.position = glm::vec3(R_RIGHT + 2.0f * BOX_WIDTH + 8.0f * GAP, TOP_BOX_BOTTOM - GAP, 5.0f);
    textureTopRight.color = WHITE;  
    textureTopRight.uv = glm::vec2(1.0f, 0.0f);
    m_vTexture.push_back(textureTopRight);

    // index 14
    textureBottomLeft.position = glm::vec3(R_RIGHT + BOX_WIDTH + 8.0f * GAP, 2.0f * TOP_BOX_BOTTOM - T_TOP + GAP, 5.0f);
    textureBottomLeft.color = WHITE;  
    textureBottomLeft.uv = glm::vec2(0.75f, 1.0f);
    m_vTexture.push_back(textureBottomLeft);

    // index 15
    textureBottomRight.position = glm::vec3(R_RIGHT + 2.0f * BOX_WIDTH + 8.0f * GAP, 2.0f * TOP_BOX_BOTTOM - T_TOP + GAP, 5.0f);
    textureBottomRight.color = WHITE;  
    textureBottomRight.uv = glm::vec2(1.0f, 1.0f);
    m_vTexture.push_back(textureBottomRight);

    m_vIndices3.push_back(12);
    m_vIndices3.push_back(13);
    m_vIndices3.push_back(15);
    m_vIndices3.push_back(12);
    m_vIndices3.push_back(14);
    m_vIndices3.push_back(15);
}

// Method to query the index of arrow button that is being clicked
int MyUtilities::queryArrowButton(float x, float y)
{
    if (x <= A_RIGHT && x >= A_LEFT)
    {
        for (int i = 0; i < 2; i++)
        {
            float yMin = m_vButtons[i * 11 + 2].position.y;
            float yMax = m_vButtons[i * 11 + 0].position.y;

            if (y <= yMax && y >= yMin)
            {
                return i;
            }
        }
    }
    return -1; // not clicking any button
}

// Method to simulate the effect of button being pressed by changing the color intensity
void MyUtilities::pressArrowButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device)
{
    if (index < 0 || index >= 2) return; 
    for (int i = index * 11; i < index * 11 + 11; i++)
    {
        m_vButtons[i].color *= intensity;
    }

    for (auto& obj : gameObjects)
    {
        if (obj.name() == std::string("arrow_button"))
        {
            MyModel::Builder builder;
            builder.vertices = m_vButtons;
            builder.indices = m_vIndices1;
            std::shared_ptr<MyModel> mybutton = std::make_shared<MyModel>(device, builder);
            obj.model = mybutton; 
        }
    }
};

// Method to query the index of range button that is being clicked
int MyUtilities::queryRangeButton(float x, float y)
{
    // All the related boxes indices: 
    // 
    // x-minus: 20-23 (button to decrease u-degree)
    // x-plus : 24-27 (button to increase u-degree)
    // y-minus: 28-31 (button to decrease v-degree)
    // y-plus : 32-35 (button to increase v-degree)

    if (x <= BUTTON_START + BUTTON_BOX_WIDTH && x >= BUTTON_START) // Minus button
    {
        if (y >= m_vRange[22].position.y && y <= m_vRange[20].position.y)
        {
            return 0; // Let index 0 denote x-minus button
        }
        else if (y >= m_vRange[30].position.y && y <= m_vRange[28].position.y)
        {
            return 2; // Let index 2 denote y-minus button
        }
    }

    else if (x <= R_RIGHT && x >= R_RIGHT - BUTTON_BOX_WIDTH)
    {
        if (y >= m_vRange[26].position.y && y <= m_vRange[24].position.y)
        {
            return 1; // Let index 1 denote x-plus button
        }
        else if (y >= m_vRange[34].position.y && y <= m_vRange[32].position.y)
        {
            return 3; // Let index 3 denote y-plus button
        }
    }
    return -1; // not clicking any button
}; 

// Increase the number of white bars by 1 if plus button is pressed and decrease if minus button is pressed. 
void MyUtilities::pressRangeButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device, bool pressed)
{
    if (index < 0 || index >= 4) return; 
    if (index == 0) // minus button for u-degree is pressed.
    {
        for (int i = 20; i < 24; i++)
        {
            m_vRange[i].color *= intensity;
        }
        for (int i = 36; i < 40; i++)
        {
            m_vRange[i].color *= intensity;
        }
        if (pressed)
        {
            u_deg -= 1;
            if (u_deg < 1) u_deg = 1; 
            else
            {
                m_vRange[60 + (u_deg * 4) + 0].color = BLACK;
                m_vRange[60 + (u_deg * 4) + 1].color = BLACK;
                m_vRange[60 + (u_deg * 4) + 2].color = BLACK;
                m_vRange[60 + (u_deg * 4) + 3].color = BLACK;
            }
        }
    }
    else if (index == 1) // plus button for u-degree is pressed.
    {
        for (int i = 24; i < 28; i++)
        {
            m_vRange[i].color *= intensity;
        }
        for (int i = 44; i < 52; i++)
        {
            m_vRange[i].color *= intensity;
        }
        if (pressed)
        {
            u_deg += 1;
            if (u_deg > MAX_DEGREE) u_deg = MAX_DEGREE; 
            else
            {
                m_vRange[60 + (u_deg - 1) * 4 + 0].color = WHITE;
                m_vRange[60 + (u_deg - 1) * 4 + 1].color = WHITE;
                m_vRange[60 + (u_deg - 1) * 4 + 2].color = WHITE;
                m_vRange[60 + (u_deg - 1) * 4 + 3].color = WHITE;
            }
        }
    }
    else if (index == 2) // minus button for v-degree is pressed.
    {
        for (int i = 28; i < 32; i++)
        {
            m_vRange[i].color *= intensity;
        }
        for (int i = 40; i < 44; i++)
        {
            m_vRange[i].color *= intensity;
        }
        if (pressed)
        {
            v_deg -= 1;
            if (v_deg < 1) v_deg = 1; 
            else
            {
                m_vRange[100 + (v_deg * 4) + 0].color = BLACK;
                m_vRange[100 + (v_deg * 4) + 1].color = BLACK;
                m_vRange[100 + (v_deg * 4) + 2].color = BLACK;
                m_vRange[100 + (v_deg * 4) + 3].color = BLACK;
            }
        }

    }
    else if (index == 3) // plus button for v-degree is pressed.
    {
        for (int i = 32; i < 36; i++)
        {
            m_vRange[i].color *= intensity;
        }
        for (int i = 52; i < 60; i++)
        {
            m_vRange[i].color *= intensity;
        }
        if (pressed)
        {
            v_deg += 1;
            if (v_deg > MAX_DEGREE) v_deg = MAX_DEGREE; 
            else
            {
                m_vRange[100 + (v_deg - 1) * 4 + 0].color = WHITE;
                m_vRange[100 + (v_deg - 1) * 4 + 1].color = WHITE;
                m_vRange[100 + (v_deg - 1) * 4 + 2].color = WHITE;
                m_vRange[100 + (v_deg - 1) * 4 + 3].color = WHITE;
            }
        }
    }

    // Update the model for the range button
    for (auto& obj : gameObjects)
    {
        if (obj.name() == std::string("range_button"))
        {
            MyModel::Builder builder;
            builder.vertices = m_vRange;
            builder.indices = m_vIndices2;
            std::shared_ptr<MyModel> myrange = std::make_shared<MyModel>(device, builder);
            obj.model = myrange; 
        }
    }
}; 

// Method to query the index of texture button that is being clicked
int MyUtilities::queryTextureButton(float x, float y)
{
    for (int i = 0; i < 4; i++)
    {
        float xMin = m_vTexture[i * 4 + 0].position.x;
        float xMax = m_vTexture[i * 4 + 3].position.x;
        float yMin = m_vTexture[i * 4 + 3].position.y;
        float yMax = m_vTexture[i * 4 + 0].position.y;

        if (yMin <= y && y <= yMax && xMin <= x && x <= xMax)
        {
            return i;
        }
    }
    return -1; // not clicking any button
}

// Method to simulate the effect of button being pressed by changing the color intensity
void MyUtilities::pressTextureButton(int index, float intensity, std::vector<MyGameObject>& gameObjects, MyDevice &device)
{
    if (index < 0 || index >= 4) return; 
    
    m_vTexture[index * 4 + 0].color *= intensity;
    m_vTexture[index * 4 + 1].color *= intensity;
    m_vTexture[index * 4 + 2].color *= intensity;
    m_vTexture[index * 4 + 3].color *= intensity;

    for (auto& obj : gameObjects)
    {
        if (obj.name() == std::string("texture_button"))
        {
            MyModel::Builder builder;
            builder.vertices = m_vTexture;
            builder.indices = m_vIndices3;
            std::shared_ptr<MyModel> mybutton = std::make_shared<MyModel>(device, builder);
            obj.textureModel = mybutton; 
        }
    }
}