#ifndef CATS2_VIEWER_WIDGET_HPP
#define CATS2_VIEWER_WIDGET_HPP

#include "ViewerPointerTypes.hpp"
#include "AgentDataItemGroup.hpp"

#include <AgentData.hpp>

#include <QtCore/QObject>
#include <QtWidgets/QWidget>
#include <QtGui/QPixmap>

class FrameScene;
class QGraphicsPixmapItem;
class QGraphicsTextItem;
class AgentTextItem;
class AgentItem;
class AnnotatedPolygonItem;
class TrajectoryItem;

namespace Ui
{
class ViewerWidget;
}

/*!
 * \brief The user interface class that shows current video frame and the detected objects positions.
 */
class ViewerWidget : public QWidget
{
    Q_OBJECT
public:
    //! Constructor.
    explicit ViewerWidget(ViewerDataPtr data, QSize frameSize, QWidget *parent = nullptr);
    //! Destructor.
    virtual ~ViewerWidget() final;

public:
    //! Sets the flag that defines if the viwer is adjusted to the view size
    //! automatically.
    void setAutoAdjust(bool value);

signals:
    //! Notifies that the mouse position has changed and sends it out in both
    //! image and world coordinates.
    void mousePosition(PositionPixels imagePosition, PositionMeters worldPosition);
    //! Notifies that the right mouse button was clicked on the scene, and sends
    //! its position out in world coordinates.
    void notifyRightButtonClick(PositionMeters worldPosition);

public slots:
    //! Triggered on arrival of the new data.
    void updateAgentLabels(QList<AgentDataWorld> agentsData);
    //! Triggered on arrival of the new data.
    void updateAgents(QList<AgentDataWorld> agentsData);
    //! Set the flag that defines if the agents must be shown.
    void setShowAgents(bool agentsShown);

    //! Request to update the trajectory on the scene.
    // TODO : not used at the moment, to implement
    void updateTrajectory(QString agentId, QQueue<PositionMeters> polygon);
    //! Request to update the target on the scene.
    void updateTarget(QString agentId, PositionMeters position);
    //! Update areas on the scene for specific agent.
    void updateAreas(QString agentId, QList<AnnotatedPolygons> polygons);
    //! Show/hides the navigation data.
    void showNavigationData(bool dataShown);

    //! Only one agent data can be shown in a time.
    void updateCurrentAgent(QString agentId);

    //! Updates the agent's color.
    void updateColor(QString id, QColor color);

    //! Updates the setup outline polygon.
    void updateSetup(AnnotatedPolygons polygon);
    //! Set the flag that defines if the setup must be shown.
    void setShowSetup(bool showSetup);

    //! Zoom on the video.
    void onZoomIn();
    //! Unzoom the video.
    void onZoomOut();
    //! Save current frame.
    void saveCurrentFrameToFile();
    //! Scale to the available area.
    void adjust();

protected slots:
    //! A new frame arrived.
    void onNewFrame(QSharedPointer<QPixmap> pixmap, int fps);
    //! Triggered when a new mouse position is received from the scene.
    void onMouseMoved(QPointF scenePosition);
    //! Triggered when a right button click is received from the scene.
    void onRightButtonClicked(QPointF scenePosition);

protected:
    //! Computes new average frame rate value as exponential moving average.
    void updateFrameRate(int fps);
    //! Converts the scene position from PositionPixels to PositionMeters.
    bool convertScenePosition(const PositionPixels& imagePosition,
                              PositionMeters& worldPosition);
    //! Converts the world position from PositionMeters to PositionPixels.
    bool convertWorldPosition(const PositionMeters& worldPosition,
                              PositionPixels& imagePosition);
    //! Shows/hide the navigation data for given agent.
    void setNavigationDataVisible(QString agentId, bool visible);

protected:
    //! Context menu.
    void contextMenuEvent(QContextMenuEvent *event) override;
    //! Resize event.
    void resizeEvent(QResizeEvent *event) override;

private: // view adjust
    //! Defines if the scene is adjusted to the view size automatically.
    bool m_autoAdjust;
    //! Actions to be used in the context menu.
    QAction* m_adjustAction;

private: // video playback data
    //! The data object that provides the frames and agent's positions to show.
    ViewerDataPtr m_data;
    //! The frame size.
    QSize m_frameSize;
    //! The form.
    Ui::ViewerWidget* m_uiViewer;
    //! The graphics scene to show the video stream and agents.
    FrameScene* m_scene;
    //! The item used to show the video stream.
    QGraphicsPixmapItem* m_videoFrame;
    //! The item used to show the video's frame rate.
    QGraphicsTextItem* m_frameRate;
    //! The average frame rate.
    double m_averageFps;

private: // agents items
    // FIXME : at the moment the agent's id is copied as it is without taking
    // into account that theoretically two agents coming from different sources
    // can have the same id.
    //! The colors of agents. Updated by robots.
    // TODO : not used at the moment, to implement
    QMap<QString, QColor> m_agentColors;

    //! The map containing the agent's id's with the corresponding label
    //!  on the scene. Updated by the tracker.
    QMap<QString, AgentTextItem*> m_agentLabels;

    //! The map containing the agent's id's with the corresponding object
    //!  on the scene. Updated by the tracker.
    QMap<QString, AgentItem*> m_agents;
    //! The flag that defines if we show agents on the map.
    bool m_showAgents;

    //! The navigation data for robots. Updated by the navigation system of the robot.
    QMap<QString, AgentDataItemGroup> m_navigationData;
    //! The flag that defines if the navigation data is to be shown on the map.
    bool m_showNavigationData;

    //! The current agent's id. Used to show the navigation data.
    QString m_currentAgentId;

    //! The map of the setup.
    AnnotatedPolygonItem* m_setupPolygon;
    //! The flag that defines if we show agents on the map.
    bool m_showSetup;
};

#endif // CATS2_VIEWER_WIDGET_HPP
