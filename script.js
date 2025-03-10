window.onload = function() {
    const url_params= new URLSearchParams(window.location.search);
    const role = url_params.get('role');

    if (document.body.id==='login-page'){
        document.getElementById('role').value = role;
        document.getElementById('login-form').addEventListener('submit', function (event){
        event.preventDefault();
        if (role){
            sessionStorage.setItem('role',role);
            event.preventDefault();
            if  (role==='mentor'){
                window.location.href = 'mentor_menteeslist.html';
            } else if (role === 'mentee'){
                window.location.href = 'main.html';
            }
        }else{
            alert('No role detected. Please select your role')
        }
        });
    }
    
    if (document.body.id === 'main-page'){
        const storedRole=sessionStorage.getItem('role');

        if (storedRole == 'mentor'){
            document.querySelectorAll('.mentee-only-btn').forEach(btn => btn.style.display='none');
        }else if(storedRole == 'mentee'){
            document.querySelectorAll('.mentor-only-btn, .mentor-only-container').forEach(elem => elem.style.display='none');
            
            const sidebar_mentee_name = document.getElementById('mentee-name');
            sidebar_mentee_name.style.backgroundColor='darkgreen';
            sidebar_mentee_name.textContent='';
        }
    }
};
function toggleEditMode(button) {
    const meetingCard = button.closest('.meeting-card');
    const noteContent = meetingCard.querySelector('.note-content');

    if (button.textContent === '✏️ Edit') {
        const textarea = document.createElement('textarea');
        textarea.value = noteContent.textContent;
        meetingCard.replaceChild(textarea, noteContent);
        button.textContent = '💾 Save';
    } else {
        const updatedNote = document.createElement('p');
        updatedNote.textContent = meetingCard.querySelector('textarea').value;
        meetingCard.replaceChild(updatedNote, meetingCard.querySelector('textarea'));
        button.textContent = '✏️ Edit'; 
    }
}

document.querySelectorAll('.edit-btn').forEach(button => {
    button.addEventListener('click', () => toggleEditMode(button));
});

function addNewMeeting() {
    const meetingDate = document.querySelector('#meeting-date').value;
    const meetingSummary = document.querySelector('#meeting-summary').value;

    if (meetingDate && meetingSummary) {
        const newMeetingCard = document.createElement('div');
        newMeetingCard.classList.add('meeting-card');
        newMeetingCard.innerHTML = `
            <h3>${meetingDate}</h3>
            <p class="note-content">${meetingSummary}</p>
            <button class="edit-btn">✏️ Edit</button>
        `;
        document.querySelector('#meeting-history').appendChild(newMeetingCard);
        newMeetingCard.querySelector('.edit-btn').addEventListener('click', () => toggleEditMode(newMeetingCard.querySelector('.edit-btn')));
        document.querySelector('#meeting-date').value = '';
        document.querySelector('#meeting-summary').value = '';
    } else {
        alert('Please fill in both the date and summary.');
    }
}

document.querySelector('#notes .save-btn').addEventListener('click', addNewMeeting);
