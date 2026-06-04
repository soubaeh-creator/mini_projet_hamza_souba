#include "StatsDialog.h"
#include <QPainter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
using namespace std;

// ─────────────────────────────────────────
// StatsWidget — Dessin de l'histogramme
// ─────────────────────────────────────────
StatsWidget::StatsWidget(const vector<shared_ptr<Student>>& students,
                         QWidget* parent)
    : QWidget(parent), students(students) {
    setMinimumSize(600, 400);
}

int StatsWidget::countRange(float min, float max) const {
    int count = 0;
    for (auto& s : students)
        if (s->getGpa() >= min && s->getGpa() < max)
            count++;
    return count;
}

void StatsWidget::paintEvent(QPaintEvent*) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int W = width();
    int H = height();

    // ── Fond blanc ──
    painter.fillRect(0, 0, W, H, Qt::white);

    // ── Titre ──
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(0, 10, W, 30, Qt::AlignCenter,
                     "Distribution des GPA");

    // ── Plages GPA ──
    struct Range {
        float   min, max;
        QString label;
        QColor  color;
    };

    vector<Range> ranges = {
        {0.0f, 1.0f, "0.0-1.0", QColor(231, 76,  60)},   // rouge
        {1.0f, 2.0f, "1.0-2.0", QColor(230,126,  34)},   // orange
        {2.0f, 3.0f, "2.0-3.0", QColor(241,196,  15)},   // jaune
        {3.0f, 3.5f, "3.0-3.5", QColor( 52,152, 219)},   // bleu
        {3.5f, 4.0f, "3.5-4.0", QColor( 39,174,  96)},   // vert
        {4.0f, 4.1f, "4.0",     QColor( 22, 160, 133)}   // teal
    };

    // ── Calcul des valeurs ──
    int maxCount = 1;
    vector<int> counts;
    for (auto& r : ranges) {
        int c = countRange(r.min, r.max);
        counts.push_back(c);
        if (c > maxCount) maxCount = c;
    }

    // ── Dimensions du graphe ──
    int marginL = 60;
    int marginR = 20;
    int marginT = 50;
    int marginB = 60;
    int graphW  = W - marginL - marginR;
    int graphH  = H - marginT - marginB;

    int nbBars  = ranges.size();
    int barW    = graphW / nbBars - 10;
    int spacing = graphW / nbBars;

    // ── Axes ──
    painter.setPen(QPen(Qt::black, 2));
    // Axe Y
    painter.drawLine(marginL, marginT, marginL, marginT + graphH);
    // Axe X
    painter.drawLine(marginL, marginT + graphH,
                     marginL + graphW, marginT + graphH);

    // ── Graduations axe Y ──
    painter.setFont(QFont("Arial", 9));
    for (int i = 0; i <= maxCount; i++) {
        int y = marginT + graphH - (i * graphH / maxCount);
        painter.setPen(QPen(Qt::gray, 1, Qt::DashLine));
        painter.drawLine(marginL, y, marginL + graphW, y);
        painter.setPen(Qt::black);
        painter.drawText(5, y - 8, marginL - 8, 16,
                         Qt::AlignRight, QString::number(i));
    }

    // ── Barres ──
    for (int i = 0; i < nbBars; i++) {
        int count  = counts[i];
        int barH   = (maxCount > 0) ? (count * graphH / maxCount) : 0;
        int x      = marginL + i * spacing + (spacing - barW) / 2;
        int y      = marginT + graphH - barH;

        // Barre colorée
        painter.setBrush(ranges[i].color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(x, y, barW, barH);

        // Contour
        painter.setPen(QPen(ranges[i].color.darker(130), 1));
        painter.drawRect(x, y, barW, barH);

        // Valeur au-dessus de la barre
        painter.setPen(Qt::black);
        painter.setFont(QFont("Arial", 10, QFont::Bold));
        if (count > 0)
            painter.drawText(x, y - 20, barW, 20,
                             Qt::AlignCenter, QString::number(count));

        // Label en dessous
        painter.setFont(QFont("Arial", 9));
        painter.drawText(x - 5, marginT + graphH + 5,
                         barW + 10, 30,
                         Qt::AlignCenter, ranges[i].label);
    }

    // ── Légende ──
    painter.setFont(QFont("Arial", 10));
    painter.setPen(Qt::black);
    painter.drawText(0, H - 20, W, 20,
                     Qt::AlignCenter,
                     QString("Total : %1 etudiant(s)").arg(students.size()));

    // ── Label axe Y ──
    painter.save();
    painter.translate(15, marginT + graphH / 2);
    painter.rotate(-90);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(-40, 0, 80, 20, Qt::AlignCenter, "Nb Etudiants");
    painter.restore();

    // ── Label axe X ──
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.drawText(marginL, H - 8, graphW, 16,
                     Qt::AlignCenter, "Plages de GPA");
}

// ─────────────────────────────────────────
// StatsDialog — Fenêtre contenant le widget
// ─────────────────────────────────────────
StatsDialog::StatsDialog(const vector<shared_ptr<Student>>& students,
                         QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Statistiques GPA");
    setMinimumSize(650, 480);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Widget histogramme
    auto* chart = new StatsWidget(students, this);
    layout->addWidget(chart);

    // Statistiques textuelles
    if (!students.empty()) {
        double total = 0;
        float  gpaMax = 0, gpaMin = 4;
        for (auto& s : students) {
            total += s->getGpa();
            if (s->getGpa() > gpaMax) gpaMax = s->getGpa();
            if (s->getGpa() < gpaMin) gpaMin = s->getGpa();
        }
        double avg = total / students.size();

        QString stats = QString(
            "Moyenne : %1   |   GPA Max : %2   |   GPA Min : %3")
            .arg(avg,  0, 'f', 2)
            .arg(gpaMax, 0, 'f', 2)
            .arg(gpaMin, 0, 'f', 2);

        QLabel* lblStats = new QLabel(stats, this);
        lblStats->setAlignment(Qt::AlignCenter);
        lblStats->setStyleSheet(
            "font-size: 13px; font-weight: bold; color: #2c3e50;"
            "padding: 8px; background: #ecf0f1; border-radius: 4px;");
        layout->addWidget(lblStats);
    }

    // Bouton fermer
    QPushButton* btnClose = new QPushButton("Fermer", this);
    btnClose->setStyleSheet(
        "background-color: #2c3e50; color: white;"
        "padding: 8px 20px; border-radius: 4px; font-weight: bold;");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
    layout->addWidget(btnClose, 0, Qt::AlignCenter);
}